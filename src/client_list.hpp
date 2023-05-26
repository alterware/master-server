#pragma once

#include "client.hpp"
#include "network_list.hpp"

class client_list : public network_list<client, 0>
{
public:
	bool find_client(uint64_t guid, const access_func& accessor);
	bool find_client(uint64_t guid, const const_access_func& accessor) const;

	using network_list::insert;
};