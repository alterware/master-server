#include <std_include.hpp>
#include "ping_handler.hpp"

#include <utils/cryptography.hpp>

void ping_handler::run_frame()
{
	auto count = 0;
	this->get_server().get_server_list().iterate([&](server_list::iteration_context& context)
	{
		auto& server = context.get();
		if (server.state == game_server::state::needs_ping)
		{
			server.state = game_server::state::pinged;
			server.challenge = utils::cryptography::random::get_challenge();
			server.heartbeat = std::chrono::high_resolution_clock::now();

			this->get_server().send(context.get_address(), "getinfo", server.challenge);

			if (20 >= ++count)
			{
				context.stop_iterating();
			}
		}
	});
}
