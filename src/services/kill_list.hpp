#pragma once
#include <network/address.hpp>

#include "../service.hpp"

class kill_list : public service
{
public:
	class kill_list_entry
	{
	public:
		kill_list_entry() = default;
		kill_list_entry(std::string ip_address, std::string reason);

		std::string ip_address_;
		std::string reason_;
	};

	kill_list(server& server);

	bool contains(const network::address& address, std::string& reason);
	void add_to_kill_list(const kill_list_entry& add);
	void remove_from_kill_list(const network::address& remove);
	void remove_from_kill_list(const std::string& remove);

private:
	using kill_list_entries = std::unordered_map<std::string, kill_list_entry>;
	utils::concurrency::container<kill_list_entries> entries_container_;

	void reload_from_disk();
	void write_to_disk();
};
