#include "WorldVariables.h"

// Initialize all world state variables to default values
WorldVariables::WorldVariables()
{
	IsDay = true;
	
	Elffor.SwordRetrieved = false;
	Elffor.NinaConversationHad = false;
	
	RoadToElffor.BattleFought = false;
}

// Serialize world state for saving
std::string WorldVariables::saveData()
{
	std::stringstream output;
	
	// Day/night state
	output << IsDay << ' ' << SUBENDMARKER << ' ';
	
	// Elffor location data
	output << Elffor.SwordRetrieved << ' ' 
	       << Elffor.NinaConversationHad << ' ' 
	       << SUBENDMARKER << ' ';
	
	// Road to Elffor data
	output << RoadToElffor.BattleFought << ' ' 
	       << SUBENDMARKER << '\n';
	
	// End marker
	output << ENDMARKER << '\n';
	
	return output.str();
}

// Deserialize world state from save file
void WorldVariables::loadData(std::string input)
{
	std::stringstream strstr(input);
	char marker;
	
	// Load day/night state
	if (strstr >> IsDay >> marker && marker != SUBENDMARKER) {
		std::cout << "Error: Invalid format in WorldVariables (IsDay section).\n";
		return;
	}
	
	// Load Elffor location data
	if (strstr >> Elffor.SwordRetrieved >> Elffor.NinaConversationHad >> marker && marker != SUBENDMARKER) {
		std::cout << "Error: Invalid format in WorldVariables (Elffor section).\n";
		return;
	}
	
	// Load Road to Elffor data
	if (strstr >> RoadToElffor.BattleFought >> marker && marker != SUBENDMARKER) {
		std::cout << "Error: Invalid format in WorldVariables (RoadToElffor section).\n";
		return;
	}
	
	// Verify end marker
	if (strstr >> marker && marker != ENDMARKER) {
		std::cout << "Error: Something went wrong with WorldVariables::loadData().\n";
	}
}