#include <std_include.hpp>
#include "info_response_command.hpp"

#include "../console.hpp"

const char* info_response_command::get_command() const
{
	return "infoResponse";
}

void info_response_command::handle_command(const network::address& target, const std::string_view& data)
{
	const auto found = this->get_server().get_server_list().find(
		target, [&data](game_server& server, const network::address& address)
		{
			utils::info_string info{data};
			const auto game = info.get("gamename");
			const auto challenge = info.get("challenge");

			const auto game_type = resolve_game_type(game);

			if (game_type == game_type::unknown)
			{
				server.state = game_server::state::dead;
				throw execution_exception{"Invalid game type: " + game};
			}

			if (server.state != game_server::state::pinged)
			{
				throw execution_exception{"Stray info response"};
			}

			if (challenge != server.challenge)
			{
				throw execution_exception{"Invalid challenge"};
			}

			const auto player_count = atoi(info.get("clients").data());
			const auto bot_count = atoi(info.get("bots").data());
			auto real_player_count = player_count - bot_count;
			real_player_count = std::clamp(real_player_count, 0, 18);

			server.registered = true;
			server.game = game_type;
			server.state = game_server::state::can_ping;
			server.protocol = atoi(info.get("protocol").data());
			server.clients = static_cast<unsigned int>(real_player_count);
			server.name = info.get("hostname");
			server.heartbeat = std::chrono::high_resolution_clock::now();
			server.info_string = std::move(info);

			console::log("Server registered for game %s (%i):\t%s\t- %s", game.data(), server.protocol,
			             address.to_string().data(), server.name.data());
		});

	if (!found)
	{
		throw execution_exception{"infoResponse without server!"};
	}
}
