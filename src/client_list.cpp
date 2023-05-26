#include "std_include.hpp"
#include "client_list.hpp"

bool client_list::find_client(const uint64_t guid, const access_func& accessor)
{
	auto found = false;

	this->iterate([&](iteration_context& context)
	{
		auto& client = context.get();
		if (client.guid == guid)
		{
			accessor(client, context.get_address());
			context.stop_iterating();
			found = true;
		}
	});

	return found;
}

bool client_list::find_client(const uint64_t guid, const const_access_func& accessor) const
{
	auto found = false;

	this->iterate([&](const iteration_context& context)
	{
		const auto& client = context.get();
		if (client.guid == guid)
		{
			accessor(client, context.get_address());
			context.stop_iterating();
			found = true;
		}
	});

	return found;
}
