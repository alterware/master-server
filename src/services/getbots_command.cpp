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
		"Bubbles",
		"Catalyst",
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
		"FragsAreUs",
		"FryTechTip",
		"Furry",
		"FutureRave",
		"Girl",
		"gsc-tool",
		"H3X1C",
		"Helium",
		"Hinder",
		"homura",
		"INeedBots",
		"INeedGames",
		"Infamous",
		"Inferno",
		"Jack Back",
		"Jebus3211",
		"JezuzLizard",
		"Jimbo",
		"Kordhell",
		"Lifeline",
		"Joel",
		"JTAG",
		"Laupetin",
		"llvm-project",
		"Loba",
		"Loona",
		"Louvenarde",
		"Mares",
		"momo5502",
		"MUPP",
		"mxve",
		"OneFourOne",
		"OpenAssetTools",
		"PeterG",
		"PottyMouth",
		"quaK",
		"RaidMax",
		"rapidjson",
		"rEvoRebreather",
		"RektInator",
		"Resxt",
		"RezTech",
		"Roxie",
		"Santi Diving",
		"sass",
		"Scarlxrd",
		"Sloth",
		"Slykuiper",
		"Snake",
		"Sparky",
		"Squirrel",
		"st0rm",
		"Valkyrie",
		"Wattson",
		"Wanted",
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
