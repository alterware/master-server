#pragma once

#include "network/address.hpp"

namespace network
{
	class socket
	{
	public:
		socket();
		~socket();

		socket(const socket& obj) = delete;
		socket& operator=(const socket& obj) = delete;

		socket(socket&& obj) noexcept;
		socket& operator=(socket&& obj) noexcept;

		bool bind(const address& target);

		void send(const address& target, const std::string& data) const;
		bool receive(address& source, std::string& data) const;

		bool set_blocking(bool blocking);

		static const bool socket_is_ready = true;
		bool sleep(std::chrono::milliseconds timeout) const;

	private:
#ifdef _WIN32
		using socklen_t = int;
#else
		using SOCKET = int;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif

		SOCKET socket_ = INVALID_SOCKET;
	};
}
