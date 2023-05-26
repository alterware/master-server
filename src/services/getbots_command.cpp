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
		"Eldor",
		"FutureRave",
		"Girl",
		"INeedBots",
		"INeedGames",
		"Infamous",
		"Jebus3211",
		"Joel",
		"JTAG",
		"Laupetin",
		"Louvenarde",
		"OneFourOne",
		"PeterG",
		"quaK",
		"RezTech",
		"sass",
		"Slykuiper",
		"st0rm",
		"xensik",
		"xoxor4d",
		"Diamante",
		"Dsso",
		"Evan",
		"FragsAreUs",
		"FryTechTip",
		"H3X1C",
		"homura",
		"Jimbo",
		"RektInator",
		"Squirrel",
	};

	std::stringstream stream{};
	for (const auto& bot : bot_names)
	{
		stream << bot << std::endl;
	}

	this->get_server().send(target, "getbotsResponse", stream.str());
	console::log("Sent bot names: %s", target.to_string().data());
}
