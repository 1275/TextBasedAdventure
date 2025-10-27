#ifndef ACTIONS_H
#define ACTIONS_H

/**
 * Action class - Represents player actions in the game.
 * Actions are commands that can be executed with associated function pointers.
 */

#include "Globals.h"
#include "Utilities.h"
#include "Player.h"
#include "WorldVariables.h"

class Action {
private:
	std::string command;      // Text command for this action
	bool showAction;     // Whether to display this action to the player
	void (*funcPtr)(Player&, WorldVariables&, bool&);  // Function to execute
public:
	Action(std::string commandIn, void (*funcPtrIn)(Player&, WorldVariables&, bool&), bool showActionIn = true) : command(commandIn), showAction(showActionIn), funcPtr(funcPtrIn) {}
	std::string getCommand(){return command;}
	bool getShowAction(){return showAction;}
	void callAction(Player &PC, WorldVariables &WorldVars){(*funcPtr)(PC, WorldVars, showAction);}
};

// Macro for defining action functions
#define FUNCACTION(NAME) void NAME(Player &PC, WorldVariables &WorldVars, bool &showAction) 

// Navigation actions
FUNCACTION(goToElfforMyHouseInterior);
FUNCACTION(goToElfforMyHouse);
FUNCACTION(goToElfforTavern);
FUNCACTION(goToElfforTavernInterior);
FUNCACTION(goToElfforGate);
FUNCACTION(goToRoadToElfforA);

// Time actions
FUNCACTION(waitUntilDay);
FUNCACTION(waitUntilNight);

// Observation actions
FUNCACTION(observeAction);
FUNCACTION(observeElfforMyHouseInterior);
FUNCACTION(observeElfforMyHouse);
FUNCACTION(observeElfforGate);
FUNCACTION(observeElfforTavern);
FUNCACTION(observeElfforTavernInterior);
FUNCACTION(observeRoadToElfforA);

// Location-specific actions
FUNCACTION(ElfforHouseSleepInBed);
FUNCACTION(ElfforHouseWaitUntilDay);
FUNCACTION(ElfforHouseWaitUntilNight);
FUNCACTION(ElfforReadSign);
FUNCACTION(ElfforEnterTavern);
FUNCACTION(ElfforTalkToTrent);
FUNCACTION(ElfforTalktoNina);
FUNCACTION(ElfforTavernWaitUntilDay);
FUNCACTION(ElfforTavernWaitUntilNight);
FUNCACTION(EnterElffor);
FUNCACTION(RoadToElfforGoDownPath);

#endif