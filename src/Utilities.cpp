#include "Utilities.h"

// Default settings for screen display
unsigned int UtilitiesOptions::screenWidth = 80;
unsigned int UtilitiesOptions::screenHeight = 24;
bool UtilitiesOptions::displayItemDescriptions = false;
bool DialogStruct::DialogEngaged = false;

// Serialize utilities options for saving
string UtilitiesOptions::saveData()
{
	stringstream output;
	output << screenWidth << ' ' << screenHeight << ' ' << displayItemDescriptions << '\n' << ENDMARKER << '\n';
	return output.str();
}

// Load utilities options from saved data
void UtilitiesOptions::loadData(string input)
{	
	stringstream strstr(input);
	strstr >> screenWidth >> screenHeight >> displayItemDescriptions;
	
	LOADDATACHECK("UtilitiesOptions")
}

// Convert Area enum to readable string
string areaToString(Area input)
{
	switch (input) {
		case ELFFORMYHOUSEINTERIOR: return "Elffor: my house (interior)";
		case ELFFORMYHOUSE:			return "Elffor: my house";
		case ELFFORGATE:			return "Elffor: gate";
		case ELFFORTAVERN:			return "Elffor: tavern";
		case ELFFORTAVERNINTERIOR:	return "Elffor: tavern (interior)";
		case ROADTOELFFORA:			return "Road to Elffor (A)";
		default:
			cout << "Error: areaToString() received improper input.\n";
			return "";
	}
}

// Display text with word wrapping to fit screen width
void display(string input, unsigned int width)
{
	const unsigned int screenWidth = (width == 0) ? UtilitiesOptions::getScreenWidth() : width;	
	unsigned int pos;
	string remainingOutput = input;
	
	while (remainingOutput.length() > screenWidth) {
		pos = remainingOutput.find('\n');
		// Check for newline within current line
		if ((pos >= 0) && (pos <= screenWidth)) {
			cout << remainingOutput.substr(0, pos + 1);
			remainingOutput.erase(0, pos + 1);
		} else {
			pos = remainingOutput.find_last_of(' ', screenWidth);
			// Handle words longer than screen width
			if ((pos <= 0) || (pos >= screenWidth)) {
				cout << remainingOutput.substr(0, screenWidth);
				remainingOutput.erase(0, screenWidth);
				while ((remainingOutput[0] == ' ') && (remainingOutput.length() > 0))
					remainingOutput.erase(0, 1);
			} else {
				// Print words without splitting them across lines
				cout << remainingOutput.substr(0, pos);
				remainingOutput.erase(0, pos);
				while ((remainingOutput[0] == ' ') && (remainingOutput.length() > 0))
					remainingOutput.erase(0, 1);
			}
			cout << '\n';
		}
	}
	cout << remainingOutput;
}

// Display dialog text and wait for user to continue
void dialog(string input)
{
	if (DialogStruct::DialogEngaged)
		cout << '\n';
	else
		DialogStruct::DialogEngaged = true;
	
	display(input);
	cout << '\n';
	enterToContinue();
}

// Truncate text to specified length, adding "..." if needed
string truncateText(string name, int length, uint quantity)
{
	stringstream output;
	
	if (quantity == 1) {
		if (name.length() <= length)
			output << name;
		else
			output << name.substr(0, length - 3) << "...";
	} else {
		// Add quantity indicator (x##) for multiple items
		if (name.length() <= (length - (4 + digits(quantity)))) {
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
	string input;
	
	getline(cin, input);
	stringstream strstr(input);
	strstr >> selection;
	
	if (!strstr)
		return -99;
	
	return selection;
}

// Wait for user to press enter
void enterToContinue()
{
	string temp;
	display("\n(press enter to continue)");
	getline(cin, temp);
}

// Case-insensitive alphabetical comparison: 1 if left < right, -1 if left > right, 0 if equal
int stringComp(string leftComp, string rightComp)
{
	int i;
	// Convert to lowercase
	for (i = 0; i < leftComp.length(); i++)
		leftComp[i] = tolower(leftComp[i]);
	for (i = 0; i < rightComp.length(); i++)
		rightComp[i] = tolower(rightComp[i]);
	
	// Compare character by character
	int compLength = min(leftComp.length(), rightComp.length());
	for (i = 0; i < compLength; i++) {
		if (leftComp[i] < rightComp[i])
			return 1;
		else if (leftComp[i] > rightComp[i])
			return -1;
	}
	
	// If all characters match, shorter string comes first
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
