#include "Game.h"
#include "Menu.h"

// Constructor - Initialize player and save initial state
Game::Game() : PC(StartingLocation), LocationVar(PC, WorldVars)
{
	saveDefaultData();
}

// Main game loop - Handle main menu and game start
void Game::run()
{
	int selection;
	std::string filename, tester;
	std::ifstream testStream;
	
	display("\nWelcome to the adventure game!\n");
	
	do {
		display("\nWhat will you do? (enter the number corresponding to your selection)\n 1. New game\n 2. Load game\n 0. Quit game\n");
		selection = getSelection();
		switch (selection) {
			case 1:
				std::cout << '\n';
				if (loadDefaultData() == Status::OK)
					playGame();
				break;
			case 0:
				break;
			case 2:
				display("\nWhat is the name of your save file? (press enter to cancel)\n");
				std::getline(std::cin, filename);
				
				if (filename != "") {
					// Verify save file exists and can be opened
					testStream.open(filename.c_str());
					if (!testStream.is_open()) {
						std::stringstream output;
						output << "\nSorry, could not open file " << filename << ". Try again.\n";
						display(output.str());
						selection = -1;
						break;
					}
					
					// Read first line to validate file format
					std::getline(testStream, tester);
					testStream.close();
					
					// Check if file is a valid save file
					if (tester != "start_save_file") {
						std::stringstream output;
						output << "\nSorry, " << filename << " is not the name of a valid save file.  Try again.\n";
						display(output.str());
						selection = -1;
						break;
					}
				
					// Load game data from file and start playing
					if (loadGame(filename) == Status::ERROR)
						std::cout << "Error: something went wrong with loadGame().\n";
					else {
						std::cout << '\n';
						playGame(filename);
					}
				}
				break;
			default:
				display("Invalid selection. Try again.\n");
				break;
		}
	} while (selection != 0);
	
	display("\nGood bye! Thanks for playing!\n\n");
}

// Helper function to write save data format to any output stream
void Game::writeSaveData(std::ostream& file)
{
	file << "start_save_file\n";
	file << UtilitiesOptions::saveData() << PC.saveData() << Menu::saveData() << WorldVars.saveData();
	file << "end_of_save_file\n";
}

void Game::saveData(std::string filename)
{
	std::ofstream file(filename.c_str());
	writeSaveData(file);
	file.close();
}

void Game::saveDefaultData()
{
	std::stringstream strstr;
	writeSaveData(strstr);
	defaultData = strstr.str();
}

Status Game::saveGame(std::string &filename)
{
	if (filename == "") {
		display("Enter a name for your save file (press enter to cancel process):\n");
		std::getline(std::cin, filename);
		if (filename == "") {
			display("No data saved.\n");
			return Status::ERROR;
		}
		else {
			std::ifstream testfile(filename.c_str());
			
			if (! testfile) {
				testfile.close();
				
				saveData(filename);
				display("\nSaved successfully (new file written).\n");
				return Status::OK;
			} else {
				std::string teststring;
				
				std::getline(testfile, teststring);
				testfile.close();
				
				if (teststring == "start_save_file") {
					
					std::stringstream tempstrstr;
					tempstrstr << "\n\"" << filename << "\" already exists as a save file. Would you like to overwrite it?\n 1. Yes\n 0. No\n";
					display(tempstrstr.str());
					
					int selection = getSelection();
					if (selection == 1) {
						saveData(filename);
						display("\nFile overwritten. Saved successfully.\n");
						return Status::OK;
					} else {
						display("\nFile not overwritten. No data saved.\n");
						filename = "";
						return Status::ERROR;
					}
				} else {
					display("\nThat file already exists, and it's not a save file, so it can't be overwritten. No data saved.\n");
					filename = "";
					return Status::ERROR;
				}
			}
		}
	} else {
		saveData(filename);
		
		std::stringstream output;
		output << "Saved successfully to file \"" << filename << "\".\n";
		display(output.str());
		
		return Status::OK;
	}
}

#define GETDATAFORLOAD \
	input.str(""); \
	do { \
		std::getline(file, tempString); \
		input << tempString << '\n'; \
	} while (tempString[0] != ENDMARKER);

#define LOADDATABODY(file, FUNCTIONNAME) \
	std::string tempString; \
	std::getline(file, tempString); \
	\
	if (tempString != "start_save_file") { \
		std::cout << "Error: " << FUNCTIONNAME << " given improper save file.\n"; \
		return Status::ERROR; \
	} \
	\
	std::stringstream input; \
	GETDATAFORLOAD \
	UtilitiesOptions::loadData(input.str()); \
	GETDATAFORLOAD \
	PC = Player(StartingLocation); \
	PC.loadData(input.str()); \
	GETDATAFORLOAD \
	Menu::loadData(input.str()); \
	GETDATAFORLOAD \
	WorldVars.loadData(input.str()); \
	std::getline(file, tempString); \
	if (tempString != "end_of_save_file") \
		std::cout << "Error:  a load function didn't reach the end of the savefile.\n"; \
	return Status::OK;
	
Status Game::loadGame(std::string filename)
{
	std::ifstream file(filename.c_str());
	LOADDATABODY(file, "loadGame")
	file.close();
	return Status::OK;  // Macro already returns on error
}

Status Game::loadDefaultData()
{
	std::stringstream file(defaultData);
	LOADDATABODY(file, "loadDefaultData")
}

void Game::playGame(std::string filename)
{	
	display("Your adventure starts. Keep your wits about you, young adventureer.\n\n");
	
	// Track last location to avoid redundant area descriptions
	lastLocation = Area::AREASTARTMARKER; // Set equal to a dummy value of the num so an area's name/description is always displayed at the start of a new game.
	std::string input;
	int savedOnLastTurn = 1; // Counter to track if game was recently saved (prevents save prompts)
	
	// Main game loop - continues until player quits or dies
	while (true) {
		// Display area description/name when entering a new location
		if (lastLocation != PC.getCurrentLocation()) {
			if(Menu::getDisplayDescription())
				LocationVar.displayDescription();
			else {
				display(areaToString(PC.getCurrentLocation()));
				std::cout << '\n';
			}
		}
		lastLocation = PC.getCurrentLocation();

		// Show available actions if enabled in settings
		if (Menu::getDisplayActions()) {
			std::cout << '\n';
			LocationVar.displayActions();
		}
		
		// Get player command
		std::cout << "\nWhat will you do?\n";
		std::getline(std::cin, input);
		std::cout << '\n';
		
		// Handle quit command
		if (input == "quit" || input == "0") {
			bool quitIt = false;
			
			// If recently saved, quit immediately
			if (savedOnLastTurn > 0) {
				quitIt = true;
			} else {
				// Prompt to save unsaved progress
				display("Would you like to save before you quit?\n 1. Yes\n 0. No\n");
				int selection = getSelection();
				if (selection == 1) {
					if (saveGame(filename) == Status::OK)
						quitIt = true;
				} else if (selection == 0)
					quitIt = true;
			}
			if (quitIt) {
				if (savedOnLastTurn <= 0)
					std::cout << '\n';
				display("And thus your adventure comes to an end for the day.\n");
				return;
			}
		}
		// Handle pause menu command
		else if (input == "menu") {
			Menu menu(PC);
			menu.pauseMenu();
		}
		// Handle save command
		else if (input == "save") {
			if (saveGame(filename) == Status::OK)
				savedOnLastTurn = 2; // Mark as saved for next 2 turns
		}
		// Process game action/command
		else {
			LocationVar.getCommand(input);
			// Check for game-ending conditions (death or termination)
			if ((PC.isDead()) || (PC.getCurrentLocation() == Area::TERMINATE)) {
				enterToContinue(); // Each deadly action should have its own output, so there's no need to define one for here.
				break;
			}
		}
		
		// Decrement save counter each turn
		if (savedOnLastTurn > 0)
			--savedOnLastTurn;
	}
}