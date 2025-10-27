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
	
	string defaultData;  // Stores initial game state
	Area lastLocation;   // Track location changes
	
	// Save/load helpers
	void writeSaveData(ostream& file);  // Write save data to stream
	void readSaveData(istream& file, const string& functionName);  // Read save data from stream
	void saveDefaultData();
	status loadDefaultData();
	
	status saveGame(string &filename);
	void saveData(string filename);
	status loadGame(string filename);
	void playGame(string filename = "");
public:
	Game();
	void run();  // Main game loop
};

#endif