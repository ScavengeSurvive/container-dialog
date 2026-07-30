#include "container-dialog.inc"

#include <test-boilerplate>
#include <test-boilerplate-items>
#include <zcmd>


new Container:container;

main() {
	container = CreateContainer("test container", 10);
}

CMD:container(playerid, params[]) {
	DisplayContainerInventory(playerid, container);
	return 1;
}

public OnPlayerOpenContainer(playerid, Container:containerid) {
	Logger_Log("OnPlayerOpenContainer",
		Logger_I("playerid", playerid),
		Logger_I("containerid", _:containerid));
}

public OnPlayerCloseContainer(playerid, Container:containerid) {
	Logger_Log("OnPlayerCloseContainer",
		Logger_I("playerid", playerid),
		Logger_I("containerid", _:containerid));
}

public OnPlayerViewContainerOpt(playerid, Container:containerid) {
	Logger_Log("OnPlayerViewContainerOpt",
		Logger_I("playerid", playerid),
		Logger_I("containerid", _:containerid));
}

public OnPlayerSelectContainerOpt(playerid, Container:containerid, option) {
	Logger_Log("OnPlayerSelectContainerOpt",
		Logger_I("playerid", playerid),
		Logger_I("containerid", _:containerid),
		Logger_I("option", option));
}

public OnMoveItemToContainer(playerid, Item:itemid, Container:containerid) {
	Logger_Log("OnMoveItemToContainer",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid),
		Logger_I("containerid", _:containerid));
}

public OnMoveItemToInventory(playerid, Item:itemid, Container:containerid) {
	Logger_Log("OnMoveItemToInventory",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid),
		Logger_I("containerid", _:containerid));
}


public OnPlayerOpenInventory(playerid) {
	Logger_Log("OnPlayerOpenInventory",
		Logger_I("playerid", playerid));
}

public OnPlayerCloseInventory(playerid) {
	Logger_Log("OnPlayerCloseInventory",
		Logger_I("playerid", playerid));
}

public OnPlayerSelectExtraItem(playerid, item) {
	Logger_Log("OnPlayerSelectExtraItem",
		Logger_I("playerid", playerid),
		Logger_I("item", item));
}

public OnPlayerViewInventoryOpt(playerid) {
	Logger_Log("OnPlayerViewInventoryOpt",
		Logger_I("playerid", playerid));
}

public OnPlayerSelectInventoryOpt(playerid, option) {
	Logger_Log("OnPlayerSelectInventoryOpt",
		Logger_I("playerid", playerid),
		Logger_I("option", option));
}

public OnPlayerAddToInventory(playerid, Item:itemid, bool:success) {
	Logger_Log("OnPlayerAddToInventory",
		Logger_I("playerid", playerid),
		Logger_I("success", success));
}

public OnPlayerAddedToInventory(playerid, Item:itemid) {
	Logger_Log("OnPlayerAddedToInventory",
		Logger_I("playerid", playerid),
		Logger_I("itemid", _:itemid));
}
