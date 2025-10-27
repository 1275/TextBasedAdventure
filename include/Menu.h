#ifndef PAUSEMENU_H
#define PAUSEMENU_H

/**
 * Menu class - Handles pause menu, options, and inventory display.
 * Manages display settings and provides player with game options.
 */

#include "Globals.h"
#include "Utilities.h"
#include "Player.h"

class Menu {
private:
	Player &PC;
	static bool DisplayDescription;  // Show item descriptions in menus
	static bool DisplayActions;      // Show available actions
	
	void optionsMenu();
	void inventoryMenu();
public:
	Menu(Player &input);
	void pauseMenu();
	
	static bool getDisplayDescription(){return DisplayDescription;}
	static bool getDisplayActions(){return DisplayActions;}
	
	static string saveData();
	static void loadData(string input);
};

#endif