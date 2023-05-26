#pragma once

#include "game_server.hpp"
#include "network_list.hpp"

class server_list : public network_list<game_server, 30>
{
public:
	void find_registered_servers(game_type game, int protocol, const access_func& accessor);
	void find_registered_servers(game_type game, int protocol, const const_access_func& accessor) const;

	void heartbeat(const network::address& address);
};
