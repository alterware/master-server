#include <std_include.hpp>
#include "getservers_command.hpp"

#include "../console.hpp"

#include <utils/parameters.hpp>

namespace
{
	struct prepared_server
	{
		uint32_t address;
		uint16_t port;
	};
}

constexpr auto MTU = 1400; // Real UDP MTU is more like 1500 bytes, but we keep a little wiggle room just in case
constexpr auto DPM_PROTOCOL_ADDRESS_LENGTH = sizeof prepared_server::address;
constexpr auto DPM_PROTOCOL_PORT_LENGTH = sizeof prepared_server::port;

static_assert(DPM_PROTOCOL_ADDRESS_LENGTH == 4);
static_assert(DPM_PROTOCOL_PORT_LENGTH == 2);

const char* getservers_command::get_command() const
{
	return "getservers";
}

void getservers_command::handle_command(const network::address& target, const std::string_view& data)
{
	const utils::parameters params(data);
	if (params.size() < 2)
	{
		throw execution_exception("Invalid parameter count");
	}

	const auto& game = params[0];

	const auto* p = params[1].data();
	char* end;
	const auto protocol = strtol(params[1].data(), &end, 10);
	if (p == end)
	{
		throw execution_exception("Invalid protocol");
	}

	const auto game_type = resolve_game_type(game);
	if (game_type == game_type::unknown)
	{
		throw execution_exception("Invalid game type: " + game);
	}

	std::queue<prepared_server> prepared_servers{};

	this->get_server().get_server_list() //
		.find_registered_servers(game_type, protocol,
			[&prepared_servers](const game_server&, const network::address& address)
		{
			const auto addr = address.get_in_addr().sin_addr.s_addr;
			const auto port = htons(address.get_port());

			prepared_servers.push({ addr, port });
		});

	size_t packet_count = 0;
	std::string response{};

	while (!prepared_servers.empty())
	{
		const auto& server = prepared_servers.front();
		response.push_back('\\');
		response.append(reinterpret_cast<const char*>(&server.address), DPM_PROTOCOL_ADDRESS_LENGTH);
		response.append(reinterpret_cast<const char*>(&server.port), DPM_PROTOCOL_PORT_LENGTH);
		prepared_servers.pop();

		if (response.size() >= MTU || prepared_servers.empty())
		{
			// Only send EOT if the queue is empty (last packet)
			if (prepared_servers.empty())
			{
				response.push_back('\\');
				response.append("EOT");
				response.push_back('\0');
				response.push_back('\0');
				response.push_back('\0');
			}

			this->get_server().send(target, "getserversResponse", response);
			packet_count++;

			response.clear();
		}
	}

	console::log("Sent %zu servers in %zu parts for game %s:\t%s", prepared_servers.size(), packet_count, game.data(), target.to_string().data());
}
