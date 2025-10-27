#include "Utilities.h"


// Default settings for screen display
unsigned int UtilitiesOptions::screenWidth = 80;
unsigned int UtilitiesOptions::screenHeight = 24;
bool UtilitiesOptions::displayItemDescriptions = false;
bool DialogStruct::DialogEngaged = false;

// Serialize utilities options for saving
std::string UtilitiesOptions::saveData()
{
	std::stringstream output;
	output << screenWidth << ' ' << screenHeight << ' ' << displayItemDescriptions << '\n' << ENDMARKER << '\n';
	return output.str();
}

// Load utilities options from saved data
void UtilitiesOptions::loadData(std::string input)
{	
	std::stringstream strstr(input);
	strstr >> screenWidth >> screenHeight >> displayItemDescriptions;
	
	LOADDATACHECK("UtilitiesOptions")
}

// Convert Area enum to readable std::string
std::string areaToString(Area input)
{
	static const std::unordered_map<Area, std::string> areaNames = {
		{Area::ELFFORMYHOUSEINTERIOR, "Elffor: my house (interior)"},
		{Area::ELFFORMYHOUSE, "Elffor: my house"},
		{Area::ELFFORGATE, "Elffor: gate"},
		{Area::ELFFORTAVERN, "Elffor: tavern"},
		{Area::ELFFORTAVERNINTERIOR, "Elffor: tavern (interior)"},
		{Area::ROADTOELFFORA, "Road to Elffor (A)"}
	};
	
	auto it = areaNames.find(input);
	if (it != areaNames.end()) {
		return it->second;
	}
	
	std::cout << "Error: areaToString() received improper input.\n";
	return "";
}

// Display text with word wrapping to fit screen width
void display(std::string input, unsigned int width)
{
    const unsigned int screenWidth = (width == 0) ? UtilitiesOptions::getScreenWidth() : width;
    size_t start = 0;
    
    while (start < input.length()) {
        // Calculate how much we can print
        size_t remaining = input.length() - start;
        size_t end = start + std::min(remaining, static_cast<size_t>(screenWidth));
        
        // Check for newline before screen width
        size_t newlinePos = input.find('\n', start);
        if (newlinePos != std::string::npos && newlinePos < end) {
            std::cout << input.substr(start, newlinePos - start + 1);
            start = newlinePos + 1;
            continue;
        }
        
        // If we're not at the end, try to break at a space
        if (end < input.length()) {
            size_t spacePos = input.find_last_of(' ', end - 1);
            if (spacePos != std::string::npos && spacePos > start) {
                end = spacePos;
            }
        }
        
        std::cout << input.substr(start, end - start) << '\n';
        
        // Skip leading spaces on next line
        start = end;
        while (start < input.length() && input[start] == ' ') {
            start++;
        }
    }
}

// Display dialog text and wait for user to continue
void dialog(std::string input)
{
	if (DialogStruct::DialogEngaged)
		std::cout << '\n';
	else
		DialogStruct::DialogEngaged = true;
	
	display(input);
	std::cout << '\n';
	enterToContinue();
}

// Truncate text to specified length, adding "..." if needed
std::string truncateText(std::string name, int length, uint quantity)
{
	std::stringstream output;
	
	if (quantity == 1) {
		if (name.length() <= static_cast<size_t>(length))
			output << name;
		else
			output << name.substr(0, length - 3) << "...";
	} else {
		// Add quantity indicator (x##) for multiple items
		if (name.length() <= static_cast<size_t>(length - (4 + digits(quantity)))) {
			output << name << " (x" << quantity << ")";
			
		} else
			output << name.substr(0, length - (7 + digits(quantity))) << "... (x" << quantity << ")";
	}
	
	return output.str();
}

// Prompt user for numeric input, return -99 if invalid
int getSelection()
{
	int selection;
	std::string input;
	
	std::getline(std::cin, input);
	std::stringstream strstr(input);
	strstr >> selection;
	
	if (!strstr)
		return -99;
	
	return selection;
}

// Wait for user to press enter
void enterToContinue()
{
	std::string temp;
	display("\n(press enter to continue)");
	std::getline(std::cin, temp);
}

// Case-insensitive alphabetical comparison: 1 if left < right, -1 if left > right, 0 if equal
int stringComp(std::string leftComp, std::string rightComp)
{
	// Convert to lowercase
	for (size_t i = 0; i < leftComp.length(); i++)
		leftComp[i] = std::tolower(leftComp[i]);
	for (size_t i = 0; i < rightComp.length(); i++)
		rightComp[i] = std::tolower(rightComp[i]);
	
	// Compare character by character
	size_t compLength = std::min(leftComp.length(), rightComp.length());
	for (size_t i = 0; i < compLength; i++) {
		if (leftComp[i] < rightComp[i])
			return 1;
		else if (leftComp[i] > rightComp[i])
			return -1;
	}
	
	// If all characters match, shorter std::string comes first
	if (leftComp.length() < rightComp.length())
		return 1;
	else if (leftComp.length() > rightComp.length())
		return -1;
	else
		return 0;
}

// Count the number of digits in a number (returns 1 for 0)
uint digits(uint input)
{
	if (input == 0)
		return 1;
	else
		return ((uint) log10(input)) + 1;
}

// Convert character to bool ('0' -> false, anything else -> true)
bool charToBool(char input)
{
	return (input != '0');
}
