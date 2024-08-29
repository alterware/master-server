#include <std_include.hpp>

#include "info_string.hpp"
#include "string.hpp"

namespace utils
{
	info_string::info_string(const std::string& buffer)
	{
		this->parse(buffer);
	}

	info_string::info_string(const std::string_view& buffer)
		: info_string(std::string{buffer})
	{
	}

	void info_string::set(const std::string& key, const std::string& value)
	{
		this->key_value_pairs_[key] = value;
	}

	std::string info_string::get(const std::string& key) const
	{
		const auto value = this->key_value_pairs_.find(key);
		if (value != this->key_value_pairs_.end())
		{
			return value->second;
		}

		return {};
	}

	void info_string::parse(std::string buffer)
	{
		if (buffer[0] == '\\')
		{
			buffer = buffer.substr(1);
		}

		auto key_values = string::split(buffer, '\\');
		for (size_t i = 0; !key_values.empty() && i < (key_values.size() - 1); i += 2)
		{
			const auto& key = key_values[i];
			const auto& value = key_values[i + 1];

			if (!this->key_value_pairs_.contains(key))
			{
				this->key_value_pairs_[key] = value;
			}
		}
	}

	std::string info_string::build() const
	{
		std::string info_string;
		for (const auto& [key, val] : this->key_value_pairs_)
		{
			info_string.push_back('\\');

			info_string.append(key);
			info_string.push_back('\\');
			info_string.append(val);
		}

		return info_string;
	}
}
