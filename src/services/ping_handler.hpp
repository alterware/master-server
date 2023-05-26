#pragma once

#include "../service.hpp"

class ping_handler : public service
{
public:
	using service::service;

	void run_frame() override;
};
