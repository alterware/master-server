#include <std_include.hpp>
#include "getbots_command.hpp"

#include "../console.hpp"

const char* getbots_command::get_command() const
{
	return "getbots";
}

void getbots_command::handle_command(const network::address& target, const std::string_view&)
{
	static const std::vector<std::string> bot_names
	{
		"aerosoul",
		"Ash",
		"asmjit",
		"avail",
		"Boston",
		"Bubbles",
		"Catalyst",
		"Cenote",
		"Chicago",
		"clang",
		"Cressi",
		"Cypress",
		"David Guetta",
		"Diamante",
		"Diavolo",
		"Dsso",
		"Eldor",
		"Evan",
		"Eve",
		"Flex190",
		"FragsAreUs",
		"FryTechTip",
		"Furry",
		"FutureRave",
		"Ginnie Springs",
		"Girl",
		"gsc-tool",
		"H3X1C",
		"Helium",
		"Hinder",
		"homura",
		"Horizon - SCR",
		"INeedBots",
		"INeedGames",
		"Infamous",
		"Inferno",
		"Jack Back",
		"Jebus3211",
		"JetFins",
		"JezuzLizard",
		"Jimbo",
		"KonaFuzzButt",
		"Kordhell",
		"King Crimson",
		"KiraFoxAF",
		"Lifeline",
		"Joel",
		"JTAG",
		"Laupetin",
		"llvm-project",
		"Loba",
		"Loona",
		"Loudward",
		"Louvenarde",
		"Mares",
		"momo5502",
		"Montreal",
		"MUPP",
		"mxve",
		"New York",
		"Nightzy",
		"OneFourOne",
		"OpenAssetTools",
		"PeterG",
		"Portobello",
		"Portofino",
		"PottyMouth",
		"quaK",
		"RaidMax",
		"rapidjson",
		"Read Sea",
		"RektInator",
		"Resxt",
		"rEvoRebreather",
		"RezTech",
		"Roxie",
		"Santi Diving",
		"sass",
		"saysupshelly",
		"Scarlxrd",
		"SCUBAPRO",
		"Sloth",
		"Slykuiper",
		"Snake",
		"Sparky",
		"Squirrel",
		"st0rm",
		"StarBerry",
		"TwinLeaf",
		"Valkyrie",
		"Wattson",
		"Wanted",
		"Wereric",
		"xensik",
		"xoxor4d",
		"ZoneBuilder",
		"ZoneTool",
	};

	std::stringstream stream{};
	for (const auto& bot : bot_names)
	{
		stream << bot << std::endl;
	}

	this->get_server().send(target, "getbotsResponse", stream.str());
	console::log("Sent bot names: %s", target.to_string().data());
}
