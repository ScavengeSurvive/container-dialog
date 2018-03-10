#include "container-dialog.inc"

#include <test-boilerplate>
#include <zcmd>


new ItemType:itemType;
new container;

main() {
	itemType = DefineItemType("Medkit", "Medkit", 11736, 1, 0.0, 0.0, 0.0, 0.004, 0.197999, 0.038000, 0.021000,  79.700012, 0.000000, 90.899978);
	container = CreateContainer("test container", 10);
}

CMD:item(playerid, params[]) {
	new
		Float:x,
		Float:y,
		Float:z;
	
	GetPlayerPos(playerid, x, y, z);

	CreateItem(itemType, x, y, z - 0.8);

	return 1;
}

CMD:container(playerid, params[]) {
	DisplayContainerInventory(playerid, container);
	return 1;
}

public OnPlayerOpenContainer(playerid, containerid) {
	log("OnPlayerOpenContainer",
		_i("playerid", playerid),
		_i("containerid", containerid));
}

public OnPlayerCloseContainer(playerid, containerid) {
	log("OnPlayerCloseContainer",
		_i("playerid", playerid),
		_i("containerid", containerid));
}

public OnPlayerViewContainerOpt(playerid, containerid) {
	log("OnPlayerViewContainerOpt",
		_i("playerid", playerid),
		_i("containerid", containerid));
}

public OnPlayerSelectContainerOpt(playerid, containerid, option) {
	log("OnPlayerSelectContainerOpt",
		_i("playerid", playerid),
		_i("containerid", containerid),
		_i("option", option));
}

public OnMoveItemToContainer(playerid, itemid, containerid) {
	log("OnMoveItemToContainer",
		_i("playerid", playerid),
		_i("itemid", itemid),
		_i("containerid", containerid));
}

public OnMoveItemToInventory(playerid, itemid, containerid) {
	log("OnMoveItemToInventory",
		_i("playerid", playerid),
		_i("itemid", itemid),
		_i("containerid", containerid));
}


public OnPlayerOpenInventory(playerid) {
	log("OnPlayerOpenInventory",
		_i("playerid", playerid));
}

public OnPlayerCloseInventory(playerid) {
	log("OnPlayerCloseInventory",
		_i("playerid", playerid));
}

public OnPlayerSelectExtraItem(playerid, item) {
	log("OnPlayerSelectExtraItem",
		_i("playerid", playerid),
		_i("item", item));
}

public OnPlayerViewInventoryOpt(playerid) {
	log("OnPlayerViewInventoryOpt",
		_i("playerid", playerid));
}

public OnPlayerSelectInventoryOpt(playerid, option) {
	log("OnPlayerSelectInventoryOpt",
		_i("playerid", playerid),
		_i("option", option));
}

public OnPlayerAddToInventory(playerid, itemid, success) {
	log("OnPlayerAddToInventory",
		_i("playerid", playerid),
		_i("success", success));
}

public OnPlayerAddedToInventory(playerid, itemid) {
	log("OnPlayerAddedToInventory",
		_i("playerid", playerid),
		_i("itemid", itemid));
}
