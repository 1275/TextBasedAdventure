#include "Location.h"
#include "Menu.h"

Location::Location(Player &PCin, WorldVariables &WorldVarsIn) : PC(PCin), WorldVars(WorldVarsIn)
{
	refreshActions();
}

#define ADDWAITDAYNIGHT(dayFunctionPointer, nightFunctionPointer) \
	Actions.push_back(Action("wait until day", dayFunctionPointer, ! WorldVars.IsDay)); \
	Actions.push_back(Action("wait until night", nightFunctionPointer, WorldVars.IsDay)); \
	waitDefined = true;

void Location::refreshActions()
{
	Actions.clear();
	Actions.push_back(Action("observe", observeAction));
	
	const Area currentLocation = PC.getCurrentLocation();
	
	// Lambda helper for simple navigation - much cleaner than separate functions!
	auto goTo = [](Area destination) {
		return [destination](Player &PC, WorldVariables &WorldVars, bool &showAction) {
			PC.setCurrentLocation(destination);
		};
	};
	
	switch (currentLocation) {
		case Area::ELFFORMYHOUSEINTERIOR:
			Actions.push_back(Action("leave house", goTo(Area::ELFFORMYHOUSE)));
			Actions.push_back(Action("sleep in bed", ElfforHouseSleepInBed));
			Actions.push_back(Action("wait until day", ElfforHouseWaitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", ElfforHouseWaitUntilNight, WorldVars.IsDay));
			break;
			
		case Area::ELFFORMYHOUSE:
			Actions.push_back(Action("enter house", goTo(Area::ELFFORMYHOUSEINTERIOR)));
			Actions.push_back(Action("go to gate", goTo(Area::ELFFORGATE)));
			Actions.push_back(Action("go to tavern", goTo(Area::ELFFORTAVERN)));
			Actions.push_back(Action("wait until day", waitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", waitUntilNight, WorldVars.IsDay));
			break;
			
		case Area::ELFFORTAVERN:
			Actions.push_back(Action("enter tavern", ElfforEnterTavern));
			Actions.push_back(Action("go to my house", goTo(Area::ELFFORMYHOUSE)));
			Actions.push_back(Action("go to gate", goTo(Area::ELFFORGATE)));
			Actions.push_back(Action("wait until day", waitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", waitUntilNight, WorldVars.IsDay));
			break;
			
		case Area::ELFFORTAVERNINTERIOR:
			Actions.push_back(Action("leave tavern", goTo(Area::ELFFORTAVERN)));
			if (WorldVars.IsDay) {
				Actions.push_back(Action("talk to Trent", ElfforTalkToTrent));
				Actions.push_back(Action("talk to Nina", ElfforTalktoNina));
			}
			Actions.push_back(Action("wait until day", ElfforTavernWaitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", ElfforTavernWaitUntilNight, WorldVars.IsDay));
			break;
			
		case Area::ELFFORGATE:
			Actions.push_back(Action("read sign", ElfforReadSign));
			Actions.push_back(Action("leave town", goTo(Area::ROADTOELFFORA)));
			Actions.push_back(Action("go to my house", goTo(Area::ELFFORMYHOUSE)));
			Actions.push_back(Action("go to tavern", goTo(Area::ELFFORTAVERN)));
			Actions.push_back(Action("wait until day", waitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", waitUntilNight, WorldVars.IsDay));
			break;
			
		case Area::ROADTOELFFORA:
			Actions.push_back(Action("enter Elffor", goTo(Area::ELFFORGATE)));
			Actions.push_back(Action("go down path", RoadToElfforGoDownPath));
			Actions.push_back(Action("wait until day", waitUntilDay, !WorldVars.IsDay));
			Actions.push_back(Action("wait until night", waitUntilNight, WorldVars.IsDay));
			break;
			
		default:
			std::cout << "Error: Location constructor called while PC was in an invalid location.\n";
			break;
	}
}

void Location::getCommand(std::string input)
{
	if (input == "actions") {
		if (Menu::getDisplayActions() == false)
			displayActions();
	} else {
		std::vector<Action>::iterator it;
		for (it = Actions.begin(); it != Actions.end(); it++) {
			if (input == it->getCommand()) {
				it->callAction(PC, WorldVars);
				refreshActions();
				DialogStruct::DialogEngaged = false;
				return;
			}
		}
		displayUnknownCommand();
	}
}

// Put a space before all displayed actions.
void Location::displayActions()
{
	std::stringstream output;
	output << "Your possible actions are:\n" << getActions() << " menu\n save\n quit\n";
	display(output.str());
}

void Location::displayDescription()
{
	std::vector<Action>::iterator it;
	for (it = Actions.begin(); it != Actions.end(); it++) {
		if (it->getCommand() == "observe") {
			it->callAction(PC, WorldVars);
			return;
		}
	}
	std::cout << "Error: " << areaToString(getArea()) << " doesn't have a defined observe function.\n";
}

std::string Location::getActions()
{
	std::stringstream output;
	std::vector<Action>::iterator it;
	for (it = Actions.begin(); it != Actions.end(); it++) {
		if (it->getShowAction())
			output << ' ' << it->getCommand() << '\n';
	}
	return output.str();
}

void Location::displayUnknownCommand()
{
	if (Menu::getDisplayActions())
		display("Whatever that is, you can't do it.\n");
	else
		display("Whatever that is, you can't do it.  Try using the \"actions\" command.\n");
}