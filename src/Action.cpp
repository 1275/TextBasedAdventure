#include "Action.h"

// ============================================================================
// HELPER FUNCTIONS - Eliminate repetition!
// ============================================================================

namespace {
	// Toggle day/night with custom messages
	void toggleTimeOfDay(WorldVariables &WorldVars, bool toDaytime, 
	                     const std::string& successMsg, const std::string& alreadyMsg) {
		if (WorldVars.IsDay != toDaytime) {
			WorldVars.IsDay = toDaytime;
			display(successMsg);
		} else {
			display(alreadyMsg);
		}
	}
}

// ============================================================================
// GENERIC ACTIONS
// ============================================================================

void waitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	toggleTimeOfDay(WorldVars, true, 
		"You wait until the sun rises.\n",
		"But it already is day...\n");
}

void waitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	toggleTimeOfDay(WorldVars, false,
		"You wait until the sun sets.\n",
		"But it is already night...\n");
}

void observeAction(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	const Area location = PC.getCurrentLocation();
	std::stringstream output;
	
	display(areaToString(location));
	std::cout << '\n';
	
	// Use a map for cleaner location descriptions
	static const std::map<Area, std::function<void(std::stringstream&, const Player&, const WorldVariables&)>> locationDescriptions = {
		{Area::ELFFORMYHOUSEINTERIOR, [](auto& out, const auto& pc, const auto& world) {
			out << "The inside of your house.  It's small, but it's home.  ";
			if (!world.Elffor.SwordRetrieved)
				out << "Your sword usually hangs on the wall, but it's missing, dust outlining where it used to be.  ";
			else
				out << "Dust outlines the spot on the wall where your sword usually hangs.  ";
			out << "There's a bed you can sleep in.  ";
		}},
		{Area::ELFFORMYHOUSE, [](auto& out, const auto& pc, const auto& world) {
			out << "Outside of your house.  It's a quaint little place.  "
			    << "You can see the tavern and the gate that leads out of town.  ";
		}},
		{Area::ELFFORGATE, [](auto& out, const auto& pc, const auto& world) {
			out << "The gate that leads out of the town, left open to allow free passage.  "
			    << "There is a sign posted on the side of the gate.  "
			    << "You can see the tavern and your house.  ";
		}},
		{Area::ELFFORTAVERN, [](auto& out, const auto& pc, const auto& world) {
			out << "Outside of the local tavern, The Infernal Woods, a small building constructed of dark wood.  "
			    << "It's a modest place, but it's well-kept.  "
			    << "You can see your house and the gate that leads out of town.  ";
		}},
		{Area::ELFFORTAVERNINTERIOR, [](auto& out, const auto& pc, const auto& world) {
			if (world.IsDay) {
				out << "Inside The Infernal Woods.  Trent, the bartender, is "
				    << (world.Elffor.SwordRetrieved ? "standing behind the bar.  " : "waving at you, trying to attract your attention.  ")
				    << "Nina is sitting at the bar, sipping a brew.  ";
			} else {
				out << "There's no one inside, as the place is closed.  ";
			}
		}},
		{Area::ROADTOELFFORA, [](auto& out, const auto& pc, const auto& world) {
			out << "Outside of the gate to Elffor.  ";
			if (!world.RoadToElffor.BattleFought)
				out << "There's a rustling coming from in the trees.  ";
			else
				out << "All is relatively still.  ";
		}}
	};
	
	auto it = locationDescriptions.find(location);
	if (it != locationDescriptions.end()) {
		it->second(output, PC, WorldVars);
	} else {
		std::cout << "Error: the observe action was called on an area it didn't know how to handle.";
	}
	
	// Handle day/night description (special case for ROADTOELFFORA)
	if (location == Area::ROADTOELFFORA) {
		if (WorldVars.IsDay) {
			output << "It is daytime.";
		} else {
			output << "It is nighttime" 
			       << (PC.isInInventory(LANTERN) ? "." : ", and you can see that it's hard to see in the woods.");
		}
	} else {
		output << (WorldVars.IsDay ? "It is daytime." : "It is nighttime.");
	}
	
	output << '\n';
	display(output.str());
}

// ============================================================================
// ELFFOR - MY HOUSE ACTIONS
// ============================================================================

void ElfforHouseSleepInBed(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	const bool wasDaytime = WorldVars.IsDay;
	WorldVars.IsDay = !WorldVars.IsDay;
	
	display(wasDaytime 
		? "You lie in bed in a state of half-sleep, dreaming vividly and just feeling very relaxed.  "
		  "When you finally force yourself out a bed, night has arrived.\n"
		: "You get a good night's rest, waking up with boundless energy, ready to greet the new day.\n");
}

void ElfforHouseWaitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	toggleTimeOfDay(WorldVars, true,
		"But why just wait when you can sleep in your comfy bed?\n",
		"But it already is day...\n");
}

void ElfforHouseWaitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	toggleTimeOfDay(WorldVars, false,
		"But why just wait when you can sleep in your comfy bed?\n",
		"But it is already night...\n");
}

// ============================================================================
// ELFFOR - GATE ACTIONS
// ============================================================================

void ElfforReadSign(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	dialog("WARNING:\n"
	       "Wild wolves have been spotted roaming outside of the town fence.  "
	       "They will attack unprotected travelers.  "
	       "If you are travelling alone, bring something to defend yourself with.");
}

// ============================================================================
// ELFFOR - TAVERN ACTIONS
// ============================================================================

void ElfforEnterTavern(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (WorldVars.IsDay) {
		PC.setCurrentLocation(Area::ELFFORTAVERNINTERIOR);
	} else {
		display("The tavern door is locked and won't budge.\n");
	}
}

void ElfforTalkToTrent(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (WorldVars.Elffor.SwordRetrieved) {
		dialog("\"Is life treating you well, friend?  I certainly hope it is.\"");
		return;
	}
	
	// First time talking - give sword back
	PC.addToInventory(RUSTYSWORD);
	WorldVars.Elffor.SwordRetrieved = true;
	
	dialog("\"I'm glad you popped in!  You left something here last night.\"\n\n"
	       "Trent hands you your sword.\n\n"
	       "\"You going somewhere?  You'll be needing that, then.  "
	       "I hear we've got wolves in our midst.\"\n\n"
	       "\"Bah!  Wolves, you say?\" Nina shouts from her stool.  "
	       "\"We haven't had a wolf problem in years, not since I showed them what's what!  "
	       "The lad'll be fine!\"\n\n"
	       "\"You need to get out of the tavern more, Nina.  These aren't the times they used to be.\"\n\n"
	       "Trent turns to face you.  \"Either way, good to have your sword back, eh, friend?\"");
}

void ElfforTalktoNina(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (!WorldVars.Elffor.SwordRetrieved) {
		dialog("\"How ye be these days, lad?\"");
		return;
	}
	
	if (!WorldVars.Elffor.NinaConversationHad) {
		WorldVars.Elffor.NinaConversationHad = true;
		dialog("\"Trent's just a worry-wort.  You were quite the fighter in your day, "
		       "if I recall correctly.\"\n\n"
		       "\"Your recall isn't what it used to be, Nina.\"\n\n"
		       "\"Are you doubting our boy's abilities, Trent?!\"\n\n"
		       "\"I'm just saying you could do with a bit less of the ale, that's all...\"");
	} else {
		dialog("\"Why, I'm sure you could take down a wolf with your bare hands, lad!\"");
	}
}

void ElfforTavernWaitUntilDay(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (!WorldVars.IsDay) {
		WorldVars.IsDay = true;
		dialog("Trent walks in and gasps when he sees you.  "
		       "\"How did you get in here?!...  Well, whatever, you're harmless enough.  "
		       "Just don't scare me like that again.\"");
	} else {
		display("But it already is day...\n");
	}
}

void ElfforTavernWaitUntilNight(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (WorldVars.IsDay) {
		WorldVars.IsDay = false;
		PC.setCurrentLocation(Area::ELFFORTAVERN);
		dialog("You stay until closing, at which point Trent leads you to the door.  "
		       "\"Sorry, friend, but you can't stay here for the night.\"");
	} else {
		display("But it is already night...\n");
	}
}

// ============================================================================
// ROAD TO ELFFOR ACTIONS
// ============================================================================

void EnterElffor(Player &PC, WorldVariables &WorldVars, bool &showAction)
{
	PC.setCurrentLocation(Area::ELFFORGATE);
	WorldVars.RoadToElffor.BattleFought = false;
}

void RoadToElfforGoDownPath(Player &PC, WorldVariables &WorldVars, bool &showAction) {
	if (WorldVars.RoadToElffor.BattleFought) {
		display("This is where you'd move to a new area.\n");
		return;
	}
	
	// Check for death conditions
	const bool isNightWithoutLight = !WorldVars.IsDay && !PC.isInInventory(LANTERN);
	const bool hasNoWeapon = !PC.isInInventory(RUSTYSWORD);
	
	if (isNightWithoutLight) {
		display("In the dark of the night, made only more impregnable by the thick woods, "
		        "you could only hear the wolf moving through the brush.  "
		        "By the time you saw him, he was already at your throat.\n");
		PC.killPlayer();
	} else if (hasNoWeapon) {
		display("A wolf leaps out from the brush and sprints towards you.  "
		        "Alas, with no means of defending yourself, you become its next dinner.\n");
		PC.killPlayer();
	} else {
		display("A wolf leaps out from the brush, intent on making you its dinner.  "
		        "Seeing the wolf coming at you, you slay the beast with your sword.  "
		        "The path is now safe to walk.\n");
		WorldVars.RoadToElffor.BattleFought = true;
	}
}