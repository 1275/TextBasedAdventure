#ifndef ACTIONS_H
#define ACTIONS_H

/**
 * Action class - Represents player actions in the game.
 * Actions are commands that can be executed with associated function callbacks.
 */

#include "Globals.h"
#include "Utilities.h"
#include "Player.h"
#include "WorldVariables.h"

class Action {
private:
	std::string command;      // Text command for this action
	bool showAction;          // Whether to display this action to the player
	std::function<void(Player&, WorldVariables&, bool&)> callback;  // Modern function wrapper
	
public:
	Action(std::string commandIn, 
	       std::function<void(Player&, WorldVariables&, bool&)> callbackIn, 
	       bool showActionIn = true) 
		: command(commandIn), showAction(showActionIn), callback(callbackIn) {}
	
	std::string getCommand() const { return command; }
	bool getShowAction() const { return showAction; }
	void callAction(Player &PC, WorldVariables &WorldVars) { callback(PC, WorldVars, showAction); }
};

// Type alias for action function signatures (cleaner than macro)
using ActionFunc = void(*)(Player&, WorldVariables&, bool&);

// ============================================================================
// ACTION FUNCTIONS 
// ============================================================================

// Generic actions
void observeAction(Player &PC, WorldVariables &WorldVars, bool &showAction);
void waitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction);
void waitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction);

// Elffor - My House
void ElfforHouseSleepInBed(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforHouseWaitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforHouseWaitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction);

// Elffor - Gate
void ElfforReadSign(Player &PC, WorldVariables &WorldVars, bool &showAction);
void EnterElffor(Player &PC, WorldVariables &WorldVars, bool &showAction);

// Elffor - Tavern
void ElfforEnterTavern(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforTalkToTrent(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforTalktoNina(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforTavernWaitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction);
void ElfforTavernWaitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction);

// Road to Elffor
void RoadToElfforGoDownPath(Player &PC, WorldVariables &WorldVars, bool &showAction);

#endif