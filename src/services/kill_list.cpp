#include "std_include.hpp"
#include "kill_list.hpp"

#include <utils/io.hpp>

constexpr auto* kill_file = "./kill.txt";

kill_list::kill_list_entry::kill_list_entry(std::string ip_address, std::string reason)
	: ip_address_(std::move(ip_address)), reason_(std::move(reason))
{
}

bool kill_list::contains(const network::address& address, std::string& reason)
{
	auto str_address = address.to_string(false);

	return this->entries_container_.access<bool>([&str_address, &reason](const kill_list_entries& entries)
	{
		if (const auto itr = entries.find(str_address); itr != entries.end())
		{
			reason = itr->second.reason_;
			return true;
		}

		return false;
	});
}

void kill_list::add_to_kill_list(const kill_list_entry& add)
{
	const auto any_change = this->entries_container_.access<bool>([&add](kill_list_entries& entries)
	{
		const auto existing_entry = entries.find(add.ip_address_);
		if (existing_entry == entries.end() || existing_entry->second.reason_ != add.reason_)
		{
			console::info("Added %s to kill list (reason: %s)", add.ip_address_.data(), add.reason_.data());
			entries[add.ip_address_] = add;
			return true;
		}

		return false;
	});

	if (!any_change)
	{
		console::info("%s already in kill list, doing nothing", add.ip_address_.data());
		return;
	}

	this->write_to_disk();
}

void kill_list::remove_from_kill_list(const network::address& remove)
{
	this->remove_from_kill_list(remove.to_string());
}

void kill_list::remove_from_kill_list(const std::string& remove)
{
	const auto any_change = this->entries_container_.access<bool>([&remove](kill_list_entries& entries)
	{
		if (entries.erase(remove))
		{
			console::info("Removed %s from kill list", remove.data());
			return true;
		}

		return false;
	});

	if (!any_change)
	{
		console::info("%s not in kill list, doing nothing", remove.data());
		return;
	}

	this->write_to_disk();
}

void kill_list::reload_from_disk()
{
	std::string contents;
	if (!utils::io::read_file(kill_file, &contents))
	{
		console::info("Could not find %s, kill list will not be loaded.", kill_file);
		return;
	}

	std::istringstream string_stream(contents);
	std::string line;

	this->entries_container_.access([&string_stream, &line](kill_list_entries& entries)
	{
		entries.clear();
		while (std::getline(string_stream, line))
		{
			if (line[0] == '#')
			{
				// comments or ignored line
				continue;
			}

			std::string ip;
			std::string comment;

			const auto index = line.find(' ');
			if (index != std::string::npos)
			{
				ip = line.substr(0, index);
				comment = line.substr(index + 1);
			}
			else
			{
				ip = line;
			}

			if (ip.empty())
			{
				continue;
			}

			// Double line breaks from windows' \r\n
			if (ip[ip.size() - 1] == '\r')
			{
				ip.pop_back();
			}

			entries.emplace(ip, kill_list_entry(ip, comment));
		}

		console::info("Loaded %zu kill list entries from %s", entries.size(), kill_file);
	});
}

void kill_list::write_to_disk()
{
	std::ostringstream stream;
	this->entries_container_.access([&stream](const kill_list_entries& entries)
	{
		for (const auto& [ip, entry] : entries)
		{
			stream << entry.ip_address_ << " " << entry.reason_ << "\n";
		}

		utils::io::write_file(kill_file, stream.str(), false);
		console::info("Wrote %s to disk (%zu entries)", kill_file, entries.size());
	});
}

kill_list::kill_list(server& server) : service(server)
{
	this->reload_from_disk();
}
