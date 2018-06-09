#include "container-dialog.inc"

#include <test-boilerplate>
#include <test-boilerplate-items>
#include <zcmd>


new Container:container;

main() {
	logger_debug("container-dialog", true);
	logger_debug("inventory-dialog", true);
	container = CreateContainer("test container", 10);
}

CMD:container(playerid, params[]) {
	DisplayContainerInventory(playerid, container);
	return 1;
}

public OnPlayerOpenContainer(playerid, Container:containerid) {
	log("OnPlayerOpenContainer",
		_i("playerid", playerid),
		_i("containerid", _:containerid));
}

public OnPlayerCloseContainer(playerid, Container:containerid) {
	log("OnPlayerCloseContainer",
		_i("playerid", playerid),
		_i("containerid", _:containerid));
}

public OnPlayerViewContainerOpt(playerid, Container:containerid) {
	log("OnPlayerViewContainerOpt",
		_i("playerid", playerid),
		_i("containerid", _:containerid));
}

public OnPlayerSelectContainerOpt(playerid, Container:containerid, option) {
	log("OnPlayerSelectContainerOpt",
		_i("playerid", playerid),
		_i("containerid", _:containerid),
		_i("option", option));
}

public OnMoveItemToContainer(playerid, Item:itemid, Container:containerid) {
	log("OnMoveItemToContainer",
		_i("playerid", playerid),
		_i("itemid", _:itemid),
		_i("containerid", _:containerid));
}

public OnMoveItemToInventory(playerid, Item:itemid, Container:containerid) {
	log("OnMoveItemToInventory",
		_i("playerid", playerid),
		_i("itemid", _:itemid),
		_i("containerid", _:containerid));
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

public OnPlayerAddToInventory(playerid, Item:itemid, bool:success) {
	log("OnPlayerAddToInventory",
		_i("playerid", playerid),
		_i("success", success));
}

public OnPlayerAddedToInventory(playerid, Item:itemid) {
	log("OnPlayerAddedToInventory",
		_i("playerid", playerid),
		_i("itemid", _:itemid));
}
