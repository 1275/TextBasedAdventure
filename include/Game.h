#ifndef GAME_H
#define GAME_H

/**
 * Game class - Main game controller.
 * Manages game state, save/load functionality, and game loop.
 */

#include "Globals.h"
#include "Utilities.h"
#include "Location.h"

class Game {
private:
	Player PC;
	WorldVariables WorldVars;
	Location LocationVar;
	
	std::string defaultData;  // Stores initial game state
	Area lastLocation;   // Track location changes
	
	// Save/load helpers
	void writeSaveData(std::ostream& file);  // Write save data to stream
	void readSaveData(std::istream& file, const std::string& functionName);  // Read save data from stream
	void saveDefaultData();
	Status loadDefaultData();
	
	Status saveGame(std::string &filename);
	void saveData(std::string filename);
	Status loadGame(std::string filename);
	void playGame(std::string filename = "");
public:
	Game();
	void run();  // Main game loop
};

#endif