// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_container_dialog
    #undef _inc_container_dialog
#endif
// custom include-guard to ensure we don't duplicate
#if defined _container_dialog_included
	#endinput
#endif
#define _container_dialog_included


#include <a_samp>
#include <container>
#include <inventory-dialog>
#include <easyDialog>
#include <YSI\y_hooks>


// Functions


forward DisplayContainerInventory(playerid, containerid);
/*
# Description:
-
*/

forward ClosePlayerContainer(playerid, call = false);
/*
# Description:
-
*/

forward GetPlayerCurrentContainer(playerid, &containerid);
/*
# Description:
-
*/

forward GetPlayerContainerSlot(playerid, &slot);
/*
# Description:
-
*/

forward AddContainerOption(playerid, option[]);
/*
# Description:
-
*/


// Events


forward OnPlayerOpenContainer(playerid, containerid);
/*
# Description:
-
*/

forward OnPlayerCloseContainer(playerid, containerid);
/*
# Description:
-
*/

forward OnPlayerViewContainerOpt(playerid, containerid);
/*
# Description:
-
*/

forward OnPlayerSelectContainerOpt(playerid, containerid, option);
/*
# Description:
-
*/

forward OnMoveItemToContainer(playerid, itemid, containerid);
/*
# Description:
-
*/

forward OnMoveItemToInventory(playerid, itemid, containerid);
/*
# Description:
-
*/


/*==============================================================================

	Setup

==============================================================================*/


static
	cnt_ItemListTotal[MAX_PLAYERS],
	cnt_CurrentContainer[MAX_PLAYERS],
	cnt_SelectedSlot[MAX_PLAYERS],
	cnt_InventoryString[MAX_PLAYERS][MAX_CONTAINER_SLOTS * (MAX_ITEM_NAME + MAX_ITEM_TEXT + 1)],
	cnt_OptionsList[MAX_PLAYERS][128],
	cnt_OptionsCount[MAX_PLAYERS],
	cnt_InventoryContainerItem[MAX_PLAYERS],
	cnt_InventoryOptionID[MAX_PLAYERS];


/*==============================================================================

	Zeroing

==============================================================================*/


hook OnPlayerConnect(playerid) {
	cnt_CurrentContainer[playerid] = INVALID_CONTAINER_ID;
}


/*==============================================================================

	Core Functions

==============================================================================*/


stock DisplayContainerInventory(playerid, containerid) {
	if(!IsValidContainer(containerid)) {
		return 1;
	}

	new
		containerSize,
		itemid,
		freeSlots,
		title[MAX_CONTAINER_NAME + 9],
		containername[MAX_CONTAINER_NAME],
		tmp[MAX_ITEM_NAME + MAX_ITEM_TEXT];
	
	GetContainerSize(containerid, containerSize);

	cnt_InventoryString[playerid][0] = EOS;
	cnt_ItemListTotal[playerid] = 0;

	for(new i; i < containerSize; i++) {
		GetContainerSlotItem(containerid, i, itemid);

		if(!IsValidItem(itemid)) {
			break;
		}

		GetItemName(itemid, tmp);

		format(cnt_InventoryString[playerid], sizeof(cnt_InventoryString[]), "%s[%02d]%s\n", cnt_InventoryString[playerid], GetItemTypeSize(GetItemType(itemid)), tmp);
		cnt_ItemListTotal[playerid]++;
	}

	GetContainerFreeSlots(containerid, freeSlots);

	for(new i; i < freeSlots; i++) {
		strcat(cnt_InventoryString[playerid], "<Empty>\n");
		cnt_ItemListTotal[playerid]++;
	}

	strcat(cnt_InventoryString[playerid], "Open Inventory");

	cnt_CurrentContainer[playerid] = containerid;

	if(CallLocalFunction("OnPlayerOpenContainer", "dd", playerid, containerid)) {
		return 2;
	}

	GetContainerName(containerid, containername);

	format(title, sizeof(title), "%s (%d/%d)", containername, containerSize - freeSlots, containerSize);

	if(strlen(cnt_InventoryString[playerid]) >= 2048) {
		err("cnt_InventoryString is over 2048 chars: %d",
			_i("length", strlen(cnt_InventoryString[playerid])));
	}

	Dialog_Show(playerid, _container_dialog, DIALOG_STYLE_LIST, title, cnt_InventoryString[playerid], "Options", "Close");

	return 0;
}

Dialog:_container_dialog(playerid, response, listitem, inputtext[]) {
	if(response) {
		if(!IsValidContainer(cnt_CurrentContainer[playerid])) {
			return 0;
		}

		// dbg("container-dialog", "listitem %d total %d itemcount %d freeslots %d", listitem, cnt_ItemListTotal[playerid], GetContainerItemCount(cnt_CurrentContainer[playerid]), GetContainerFreeSlots(cnt_CurrentContainer[playerid]));

		if(listitem >= cnt_ItemListTotal[playerid]) {
			DisplayPlayerInventory(playerid);
		} else {
			if(!(0 <= listitem < MAX_CONTAINER_SLOTS)) {
				err("invalid listitem value", _i("listitem", listitem));
				return 0;
			}

			new itemid;
			new ret = GetContainerSlotItem(cnt_CurrentContainer[playerid], listitem, itemid);

			if(ret != 0 || !IsValidItem(itemid)) {
				DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
			} else {
				cnt_SelectedSlot[playerid] = listitem;
				_container_displayOptions(playerid, listitem);
			}
		}
	} else {
		ClosePlayerContainer(playerid, true);
	}

	return 1;
}

stock ClosePlayerContainer(playerid, call = false) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	if(cnt_CurrentContainer[playerid] == INVALID_CONTAINER_ID) {
		return 2;
	}

	if(call) {
		if(CallLocalFunction("OnPlayerCloseContainer", "dd", playerid, cnt_CurrentContainer[playerid])) {
			DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
			return 3;
		}
	}

	ShowPlayerDialog(playerid, -1, 0, NULL, NULL, NULL, NULL);
	cnt_CurrentContainer[playerid] = INVALID_CONTAINER_ID;

	return 0;
}

stock GetPlayerCurrentContainer(playerid, &containerid) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	containerid = cnt_CurrentContainer[playerid];
	return 0;
}

stock GetPlayerContainerSlot(playerid, &slot) {
	if(!IsPlayerConnected(playerid)) {
		return 1;
	}

	slot = cnt_SelectedSlot[playerid];
	return 0;
}

stock AddContainerOption(playerid, option[]) {
	if(strlen(cnt_OptionsList[playerid]) + strlen(option) > sizeof(cnt_OptionsList[])) {
		return -1;
	}

	strcat(cnt_OptionsList[playerid], option);
	strcat(cnt_OptionsList[playerid], "\n");

	return cnt_OptionsCount[playerid]++;
}


/*==============================================================================

	Internal Functions and Hooks

==============================================================================*/


_container_displayOptions(playerid, slotid) {
	new
		tmp[MAX_ITEM_NAME + MAX_ITEM_TEXT],
		itemid;
	
	GetContainerSlotItem(cnt_CurrentContainer[playerid], slotid, itemid);

	GetItemName(itemid, tmp);

	cnt_OptionsList[playerid] = "Equip\nMove to inventory\n";
	cnt_OptionsCount[playerid] = 0;

	CallLocalFunction("OnPlayerViewContainerOpt", "dd", playerid, cnt_CurrentContainer[playerid]);

	Dialog_Show(playerid, _container_optionsList, DIALOG_STYLE_LIST, tmp, cnt_OptionsList[playerid], "Accept", "Back");

	return 0;
}

Dialog:_container_optionsList(playerid, response, listitem, inputtext[]) {
	if(!response) {
		DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
		return 1;
	}

	switch(listitem) {
		case 0: {
			if(GetPlayerItem(playerid) == INVALID_ITEM_ID) {
				new
					itemid,
					ret;

				ret = GetContainerSlotItem(cnt_CurrentContainer[playerid], cnt_SelectedSlot[playerid], itemid);
				if(ret) {
					err("current container invalid in dialog call",
						_i("containerid", cnt_CurrentContainer[playerid]),
						_i("playerid", playerid));
					ShowActionText(playerid, "An error occurred", 3000, 200);
					return 1;
				}

				RemoveItemFromContainer(cnt_CurrentContainer[playerid], cnt_SelectedSlot[playerid], playerid);
				GiveWorldItemToPlayer(playerid, itemid);
				DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
			} else {
				ShowActionText(playerid, "You are already holding something", 3000, 200);
				DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
			}
		}

		case 1: {
			new
				itemid,
				ret;

			ret = GetContainerSlotItem(cnt_CurrentContainer[playerid], cnt_SelectedSlot[playerid], itemid);
			if(ret) {
				err("current container invalid in dialog call",
					_i("containerid", cnt_CurrentContainer[playerid]),
					_i("playerid", playerid));
				ShowActionText(playerid, "An error occurred", 3000, 200);
				return 1;
			}

			if(!IsValidItem(itemid)) {
				DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
				return 0;
			}

			if(CallLocalFunction("OnMoveItemToInventory", "ddd", playerid, itemid, cnt_CurrentContainer[playerid]))
				return 0;

			new required = AddItemToInventory(playerid, itemid);

			if(required > 0) {
				new str[32];
				format(str, sizeof(str), "Extra %d slots required", required);
				ShowActionText(playerid, str, 3000, 150);
			} else if(required == 0) {
				new slot;
				GetItemContainerSlot(itemid, slot);
				RemoveItemFromContainer(cnt_CurrentContainer[playerid], slot, playerid);
			}

			DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);

			return 1;
		}

		default: {
			CallLocalFunction("OnPlayerSelectContainerOpt", "ddd", playerid, cnt_CurrentContainer[playerid], listitem - 2);
		}
	}

	return 1;
}

hook OnPlayerViewInvOpt(playerid) {
	if(cnt_CurrentContainer[playerid] != INVALID_CONTAINER_ID) {
		new
			containerName[MAX_CONTAINER_NAME],
			str[8 + MAX_CONTAINER_NAME];

		GetContainerName(cnt_CurrentContainer[playerid], containerName);

		str = "Move to ";
		strcat(str, containerName);
		cnt_InventoryOptionID[playerid] = AddInventoryOption(playerid, str);
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}

hook OnPlayerSelectInvOpt(playerid, option) {
	if(cnt_CurrentContainer[playerid] != INVALID_CONTAINER_ID) {
		if(option == cnt_InventoryOptionID[playerid]) {
			new
				slot,
				itemid;

			slot = GetPlayerSelectedInventorySlot(playerid);
			GetInventorySlotItem(playerid, slot, itemid);

			if(IsContainerFull(cnt_CurrentContainer[playerid])) {
				ShowActionText(playerid, "Container full", 3000, 200);
			}

			if(!IsValidItem(itemid)) {
				DisplayPlayerInventory(playerid);
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			new required = AddItemToContainer(cnt_CurrentContainer[playerid], itemid, playerid);

			if(required == 0) {
				if(CallLocalFunction("OnMoveItemToContainer", "ddd", playerid, itemid, cnt_CurrentContainer[playerid]))
					return Y_HOOKS_CONTINUE_RETURN_0;
			}

			DisplayPlayerInventory(playerid);

			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}


hook OnPlayerOpenInventory(playerid) {
	if(IsValidContainer(cnt_CurrentContainer[playerid])) {
		new str[8 + MAX_CONTAINER_NAME];

		GetContainerName(cnt_CurrentContainer[playerid], str);
		strcat(str, " >");

		cnt_InventoryContainerItem[playerid] = AddInventoryListItem(playerid, str);
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}


hook OnPlayerSelectExtraItem(playerid, item) {
	if(IsValidContainer(cnt_CurrentContainer[playerid])) {
		if(item == cnt_InventoryContainerItem[playerid]) {
			DisplayContainerInventory(playerid, cnt_CurrentContainer[playerid]);
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_0;
}
