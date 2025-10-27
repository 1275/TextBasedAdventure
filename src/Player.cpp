#include "Player.h"

// Constructor - Initialize player at starting location
Player::Player(Area currentLocationIn)
{
	currentLocation = currentLocationIn;
	isAlive = true;
}

// Add item(s) to inventory - unique items only added once
void Player::addToInventory(Item input, uint quantity)
{
	if (input.isUnique()) {
		if (! isInInventory(input))
			Inventory.insert(pair<string, Item> (input.getName(), input));
	} else {
		for (uint i = 0; i < quantity; i++)
			Inventory.insert(pair<string, Item> (input.getName(), input));
	}
}

// Retrieve an item from inventory by name
Item Player::getItem(string name) const
{
	if (isInInventory(name))
		return Inventory.find(name)->second;
	else {
		cout << "Error: getItem() given input on an item that wasn't in the inventory.\n";
		return ERRORITEM;
	}
}

// Check if item is in inventory (by Item object)
bool Player::isInInventory(Item input) const
{
	if (numInInventory(input) > 0)
		return true;
	else
		return false;
}

// Check if item is in inventory (by name)
bool Player::isInInventory(string name) const
{
	if (numInInventory(name) > 0)
		return true;
	else
		return false;
}

// Count how many of this item are in inventory (by Item object)
uint Player::numInInventory(Item input) const
{
	return (uint) Inventory.count(input.getName());
}

// Count how many of this item are in inventory (by name)
uint Player::numInInventory(string name) const
{
	return (uint) Inventory.count(name);
}

// Remove item(s) from inventory - returns remaining count or -1 if failed
int Player::removeFromInventory(Item input, uint quantity)
{
	uint count = (uint) numInInventory(input);
	if (count < quantity)
		return -1;
	else {
		multimap<string, Item, ItemComp>::iterator It = Inventory.lower_bound(input.getName());
		multimap<string, Item, ItemComp>::iterator ItEnd = It;
		for (uint i = 0; i < quantity; i++) {
			ItEnd++;
		}
		Inventory.erase(It, ItEnd);
		
		return (count - quantity);
	}
}

int Player::removeFromInventory(string name, uint quantity)
{
	uint count = (uint) numInInventory(name);
	if (count < quantity)
		return -1;
	else {
		multimap<string, Item, ItemComp>::iterator It = Inventory.lower_bound(name);
		multimap<string, Item, ItemComp>::iterator ItEnd = It;
		for (uint i = 0; i < quantity; i++) {
			ItEnd++;
		}
		Inventory.erase(It, ItEnd);
		
		return (count - quantity);
	}
}

// Convert inventory to formatted string for display
string Player::inventoryToString() const
{	
	if (Inventory.empty())
		return " empty inventory\n";
	
	stringstream output;
	const uint width = UtilitiesOptions::getScreenWidth();
	multimap<string, Item, ItemComp>::const_iterator it;
	int i, quantity;

	for (it = Inventory.begin() ; it != Inventory.end(); it++) {
		const string lastItem = it->first;
		
		// Count repeats of current item
		quantity = 0;
		while (it != Inventory.end()) {
			if (it->first == lastItem) {
				quantity++;
				it++;
			} else
				break;
		}
		it--;
		
		output << ' ' << truncateText(it->first, width - 1, quantity) << '\n';
		if (UtilitiesOptions::getDisplayItemDescriptions())
			output << "   " << truncateText((it->second).getDescription(), width - 3) << '\n';
	}
	
	return output.str();
}

// Serialize player data for saving
string Player::saveData()
{
	stringstream output;
	output << currentLocation << '\n';
	
	// Save each inventory item
	multimap<string, Item, ItemComp>::const_iterator it;
	for (it = Inventory.begin() ; it != Inventory.end(); it++)
		output << (it->second).getName() << '\n' << (it->second).isUnique() << '\n' << (it->second).getDescription() << '\n';
	output << "end_of_inventory" << '\n' << ENDMARKER << '\n'; 
	
	return output.str();
}

// Load player data from saved game
void Player::loadData(string input)
{
	stringstream strstr(input);
	
	int tempArea;
	strstr >> tempArea;
	currentLocation = (Area) tempArea;
	strstr.ignore(1);
	
	string tempName, tempDescription;
	bool tempUnique;
	
	getline(strstr, tempName);
	while (tempName != "end_of_inventory") {
		strstr >> tempUnique;
		strstr.ignore(1);
		getline(strstr, tempDescription);
		addToInventory(Item(tempName, tempDescription, tempUnique));
		getline(strstr, tempName);
	}
	
	LOADDATACHECK("Player")
}
