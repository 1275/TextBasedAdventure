#ifndef PLAYER_H
#define PLAYER_H

/**
 * Player class - Manages player state, inventory, and location.
 * Handles item collection, removal, and inventory queries.
 */

#include "Globals.h"
#include "Utilities.h"
#include "Item.h"

// Custom comparator for sorting items alphabetically in inventory
struct ItemComp {
	bool operator() (const std::string& leftComp, const std::string& rightComp) const {return stringComp(leftComp, rightComp) == 1;}
};

class Player {
private:
	bool isAlive;
	Area currentLocation;
	std::multimap<std::string, Item, ItemComp> Inventory;  // Allows multiple items with same name
public:
	Player(Area currentLocationIn);
	
	// Location management
	Area getCurrentLocation(){return currentLocation;}
	void setCurrentLocation(Area input){currentLocation = input;}
	
	// Life Status
	bool isDead(){return !isAlive;}
	void killPlayer(){isAlive = false;}
	
	// Inventory management
	void addToInventory(Item input, uint quantity = 1);
	Item getItem(std::string name) const;
	bool isInInventory(Item input) const;
	bool isInInventory(std::string name) const;
	uint numInInventory(Item input) const;  // Returns count of item in inventory
	uint numInInventory(std::string name) const;
	bool emptyInventory() const {return Inventory.empty();}
	int removeFromInventory(Item input, uint quantity = 1);  // Returns remaining count, -1 if failed
	int removeFromInventory(std::string name, uint quantity = 1);
	std::string inventoryToString() const;
	
	// Save/load functionality
	std::string saveData();
	void loadData(std::string input);
};

#endif