#include <std_include.hpp>
#include "elimination_handler.hpp"

constexpr auto T7_PROTOCOL = 7;

constexpr size_t MAX_SERVERS_PER_GAME = 15;

void elimination_handler::run_frame()
{
	std::unordered_map<game_type, std::unordered_map<network::address, size_t>> server_count;

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
			console::info("Removing T7 server '%s' because protocol %i is less than %i\n",
			              context.get_address().to_string().data(), server.protocol, T7_PROTOCOL);
#endif
			context.remove();
		}

		++server_count[server.game][context.get_address()];
		if (server_count[server.game][context.get_address()] >= MAX_SERVERS_PER_GAME)
		{
#ifdef _DEBUG
			console::info("Removing server '%s' because it exceeds MAX_SERVERS_PER_GAME\n",
			              context.get_address().to_string().data());
#endif
			context.remove();
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
