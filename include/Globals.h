/**
 * Global definitions, constants, and types used throughout the game.
 * This header is included by most source files.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

// Standard library includes
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

// Type aliases
using uint = unsigned int;

// Game areas/locations
enum class Area {
	TERMINATE = -1,
	AREASTARTMARKER,
	ELFFORMYHOUSEINTERIOR,
	ELFFORMYHOUSE,
	ELFFORGATE,
	ELFFORTAVERN,
	ELFFORTAVERNINTERIOR,
	ROADTOELFFORA,
	AREAENDMARKER
};

constexpr Area StartingLocation = Area::ELFFORMYHOUSEINTERIOR;

// Status codes for operations
enum class Status {
	ERROR = 0,
	OK = 1
};

// Save file format markers
constexpr char ENDMARKER = '%';
constexpr char SUBENDMARKER = '$';

// Helper function for menu display (converts bool to "On"/"Off")
inline const char* OnOff(bool value) {
	return value ? "On" : "Off";
}

// Validation macro for load functions (checks for proper end marker)
// Note: Assumes std::stringstream variable named 'strstr' exists in scope
#define LOADDATACHECK(CLASSNAME) \
	char endTest; \
	strstr >> endTest; \
	if (endTest != ENDMARKER) \
		std::cout << "Error: Something went wrong with " << CLASSNAME << "::loadData().\n";

#endif