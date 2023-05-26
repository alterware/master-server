#include <std_include.hpp>
#include "statistics_handler.hpp"
#include "../console.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

namespace
{
	void write_stats(const std::map<game_type, std::vector<std::pair<std::string, network::address>>>& servers,
	                 std::map<game_type, uint32_t>& players)
	{
		rapidjson::Document root{};
		root.SetObject();

		for (const auto& game_servers : servers)
		{
			const auto server_count = static_cast<uint32_t>(game_servers.second.size());
			const auto player_count = players[game_servers.first];

			rapidjson::Value game{};
			game.SetObject();

			game.AddMember("servers", server_count, root.GetAllocator());
			game.AddMember("players", player_count, root.GetAllocator());

			auto game_name = resolve_game_type_name(game_servers.first);
			game_name = utils::string::to_lower(game_name);

			rapidjson::Value game_name_object(game_name, root.GetAllocator());
			root.AddMember(game_name_object, game, root.GetAllocator());
		}

		rapidjson::StringBuffer root_buffer{};
		rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::ASCII<>>
			root_writer(root_buffer);
		root.Accept(root_writer);

		std::string root_data(root_buffer.GetString(), root_buffer.GetLength());
		utils::io::write_file("/var/www/master.xlabs.dev/stats.json", root_data);
	}
}

statistics_handler::statistics_handler(server& server)
	: service(server)
	  , last_print(std::chrono::high_resolution_clock::now())
{
}

void statistics_handler::run_frame()
{
	const auto now = std::chrono::high_resolution_clock::now();
	if (now - this->last_print < 5min)
	{
		return;
	}

	std::map<game_type, uint32_t> players{};
	std::map<game_type, std::vector<std::pair<std::string, network::address>>> servers;

	this->last_print = std::chrono::high_resolution_clock::now();
	this->get_server().get_server_list().iterate([&servers, &players](const server_list::iteration_context& context)
	{
		const auto& server = context.get();
		if (server.registered)
		{
			servers[server.game].emplace_back(server.name, context.get_address());
			players[server.game] += server.clients;
		}
	});

	console::lock _{};

	for (const auto& game_servers : servers)
	{
		console::log("%s (%d):", resolve_game_type_name(game_servers.first).data(),
		             static_cast<uint32_t>(game_servers.second.size()));

		for (const auto& server : game_servers.second)
		{
			console::log("\t%s\t%s", server.second.to_string().data(), server.first.data());
		}
	}

	write_stats(servers, players);
}
