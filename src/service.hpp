#pragma once

#include "network/address.hpp"

class server;

class service
{
public:
	class execution_exception : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	service(server& server)
		: server_(server)
	{
	}

	virtual ~service() = default;

	service(service&&) = delete;
	service(const service&) = delete;
	service& operator=(service&&) = delete;
	service& operator=(const service&) = delete;

	[[nodiscard]] virtual const char* get_command() const { return nullptr; }

	virtual void handle_command([[maybe_unused]] const network::address& target,
	                            [[maybe_unused]] const std::string_view& data)
	{
	}

	virtual void run_frame()
	{
	}

protected:
	[[nodiscard]] const server& get_server() const
	{
		return server_;
	}

	server& get_server()
	{
		return server_;
	}

private:
	server& server_;
};

// server and service have a cycle, but fuck, it's easier that way
// include guards should handle that
#include "server.hpp"
