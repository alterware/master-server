#pragma once

#include "utils/cryptography.hpp"
#include "game_server.hpp"

struct client
{
	enum class state
	{
		can_authenticate = 0,
		key_received,
		challenge_sent,
	};
	
	uint64_t guid{0};
	bool registered{false};
	game_type game{game_type::unknown};
	state state{state::can_authenticate};
	utils::cryptography::ecc::key key{};
	std::string challenge{};
	std::string aes_key{};
	std::chrono::high_resolution_clock::time_point heartbeat{};
};
