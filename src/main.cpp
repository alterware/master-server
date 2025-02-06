#include <std_include.hpp>

#include "console.hpp"
#include "server.hpp"
#include "crypto_key.hpp"

namespace
{
	void unsafe_main(const uint16_t port)
	{
		crypto_key::get();

		console::log("Creating socket on port %hu", port);

		network::address a{"0.0.0.0"};
		a.set_port(port);
		server s{a};

		console::signal_handler handler([&s]()
		{
			s.stop();
		});

		s.run();

		console::log("Terminating server...");
	}
}


int main(const int argc, const char** argv)
{
	console::set_title("AlterWare Master");
	console::log("AlterWare Master");

	try
	{
		unsafe_main(argc > 1 ? static_cast<uint16_t>(std::strtol(argv[1], nullptr, 10)) : 20810);
	}
	catch (std::exception& e)
	{
		console::error("Fatal error: %s", e.what());
		return -1;
	}

	return 0;
}
