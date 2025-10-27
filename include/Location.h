#ifndef LOCATION_H
#define LOCATION_H

/**
 * Location class - Manages current location and available actions.
 * Handles action display, command processing, and location descriptions.
 * 
 * To add a new location:
 *   - Add to Area enum in Globals.h (before Area::AREAENDMARKER)
 *   - Add to areaToString() in Utilities.cpp
 *   - Add static variables to WorldVariables in WorldVariables.h
 *   - Add to all functions in WorldVariables.cpp
 *   - Add description to observeAction() in Action.cpp
 *   - Create goTo and location-specific functions in Action.h/.cpp
 *   - Add actions to adjacent areas in Location constructor
 */

#include "Globals.h"
#include "Utilities.h"
#include "Action.h"

class Location {
private:
	void displayUnknownCommand();  // Show error for invalid commands

	WorldVariables &WorldVars;
	Player &PC;
	std::vector<Action> Actions;  // Available actions at current location
	
	std::string getActions();  // Get formatted list of possible actions
public:
	Location(Player &PCin, WorldVariables &WorldVarsIn);

	Area getArea(){return PC.getCurrentLocation();}
	void refreshActions();     // Update available actions based on game state
	void displayActions();     // Show all possible actions to player
	void getCommand(std::string input);  // Process player command
	void displayDescription(); // Show area description
};

#endif