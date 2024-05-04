#include <std_include.hpp>
#include "elimination_handler.hpp"

#include <utils/string.hpp>

namespace
{
	constexpr auto T7_PROTOCOL = 7;

	constexpr size_t MAX_SERVERS_PER_GAME = 10;

	std::array bad_names =
	{
		"http",
		"discord",
	};
}

void elimination_handler::run_frame()
{
	std::unordered_map<game_type, std::unordered_map<std::string, size_t>> server_count;

	auto now = std::chrono::high_resolution_clock::now();
	this->get_server().get_server_list().iterate([&](server_list::iteration_context& context)
	{
		auto& server = context.get();
		const auto diff = now - server.heartbeat;

		if ((server.state == game_server::state::pinged && diff > 2min) ||
			(server.state == game_server::state::can_ping && diff > 15min))
		{
			context.remove();
		}

		if (server.game == game_type::unknown)
		{
			return;
		}

		if (server.game == game_type::t7 && server.protocol < T7_PROTOCOL)
		{
#ifdef _DEBUG
			console::log("Removing T7 server '%s' because they are using an outdated protocol (%i)", context.get_address().to_string().data(), server.protocol);
#endif
			context.remove();
		}

		++server_count[server.game][context.get_address().to_string(false)];
		if (server_count[server.game][context.get_address().to_string(false)] > MAX_SERVERS_PER_GAME)
		{
			console::log("Removing server '%s' because it exceeds MAX_SERVERS_PER_GAME", context.get_address().to_string().data());
			context.remove();
		}

		const auto name = utils::string::to_lower(server.name);
		for (const auto& entry : bad_names)
		{
			if (const auto pos = name.find(entry); pos != std::string::npos)
			{
				console::log("Removing server '%s' (%s) because it contains a bad name", server.name.data(), context.get_address().to_string().data());
				context.remove();
			}
		}
	});

	now = std::chrono::high_resolution_clock::now();
	this->get_server().get_client_list().iterate([&](client_list::iteration_context& context)
	{
		auto& client = context.get();
		const auto diff = now - client.heartbeat;

		if (diff > 5min || (!client.registered && diff > 20s))
		{
			context.remove();
		}
	});
}
