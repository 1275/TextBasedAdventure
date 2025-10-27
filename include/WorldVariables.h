#ifndef WORLDVARIABLES_H
#define WORLDVARIABLES_H

/**
 * WorldVariables - Tracks global game state and location-specific flags.
 * Stores persistent information about time, NPCs, and events.
 */

#include "Globals.h"
#include "Utilities.h"

struct WorldVariables {
	bool IsDay;  // Current time of day
	
	// Elffor location flags
	struct {
		bool SwordRetrieved;       // Player picked up rusty sword
		bool NinaConversationHad;  // Player talked to Nina
	} Elffor;
	
	// Road to Elffor flags
	struct {	
		bool BattleFought;  // Combat encounter completed
	} RoadToElffor;
	
	WorldVariables();
	std::string saveData();
	void loadData(std::string input);
};

#endif