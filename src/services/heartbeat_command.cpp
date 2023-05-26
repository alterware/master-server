#include <std_include.hpp>
#include "heartbeat_command.hpp"

const char* heartbeat_command::get_command() const
{
	return "heartbeat";
}

void heartbeat_command::handle_command(const network::address& target, [[maybe_unused]] const std::string_view& data)
{
	this->get_server().get_server_list().heartbeat(target);
}
