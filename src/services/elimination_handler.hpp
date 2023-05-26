#pragma once

#include "../service.hpp"

class elimination_handler : public service
{
public:
	using service::service;

	void run_frame() override;
};
