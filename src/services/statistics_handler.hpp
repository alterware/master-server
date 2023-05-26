#pragma once

#include "../service.hpp"

class statistics_handler : public service
{
public:
	statistics_handler(server& server);

	void run_frame() override;

private:
	std::chrono::high_resolution_clock::time_point last_print;
};
