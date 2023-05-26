#pragma once

#include "network/socket.hpp"


class server_base
{
public:
	server_base(const network::address& bind_addr);
	virtual ~server_base() = default;

	void run();
	void stop();

	void send(const network::address& target, const std::string& command, const std::string& data,
	          const std::string& separator = " ") const;

protected:
	virtual void run_frame() = 0;
	virtual void handle_command(const network::address& target, const std::string_view& command,
	                            const std::string_view& data) = 0;

private:
	network::socket socket_{};
	volatile bool stopped_ = false;

	void run_socket();
	[[nodiscard]] bool receive_data();
	void parse_data(const network::address& target, const std::string_view& data);
};
