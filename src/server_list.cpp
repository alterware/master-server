#include "std_include.hpp"
#include "server_list.hpp"

void server_list::find_registered_servers(const game_type game, const int protocol, const access_func& accessor)
{
	this->iterate([&](iteration_context& context)
	{
		auto& server = context.get();
		if (server.registered && server.game == game && server.protocol == protocol)
		{
			accessor(server, context.get_address());
		}
	});
}

void server_list::find_registered_servers(const game_type game, const int protocol,
                                          const const_access_func& accessor) const
{
	this->iterate([&](const iteration_context& context)
	{
		const auto& server = context.get();
		if (server.registered && server.game == game && server.protocol == protocol)
		{
			accessor(server, context.get_address());
		}
	});
}

void server_list::heartbeat(const network::address& address)
{
	this->insert(address, [](game_server& server)
	{
		if (server.state == game_server::state::can_ping)
		{
			server.heartbeat = std::chrono::high_resolution_clock::now();
			server.state = game_server::state::needs_ping;
		}
	});
}
