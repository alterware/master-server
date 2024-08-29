#include <std_include.hpp>
#include "env.hpp"

namespace utils::env
{
	std::string get_value(const std::string& name)
	{
		const char* var_value = nullptr;
#ifdef _WIN32

		char* buffer = nullptr;
		size_t len = 0;
		if (_dupenv_s(&buffer, &len, name.c_str()) == 0 && buffer != nullptr)
		{
			var_value = buffer;
		}

		const auto _ = gsl::finally([&]
		{
			std::free(buffer);
		});

		if (var_value)
		{
			return var_value;
		}

		return {};
#else
		var_value = std::getenv(name.c_str());
		return var_value ? std::string{ var_value } : std::string{};
#endif
	}
}
