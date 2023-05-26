#include <std_include.hpp>

#include "parameters.hpp"
#include "string.hpp"

namespace utils
{
	parameters::parameters(std::string buffer)
	{
		while (!buffer.empty() && (buffer.back() == '\0' || isspace(buffer.back())))
		{
			buffer.pop_back();
		}

		this->arguments_ = string::split(buffer, ' ');
	}

	parameters::parameters(const std::string_view& buffer)
		: parameters(std::string{buffer.data(), buffer.size()})
	{
	}

	void parameters::add(std::string value)
	{
		this->arguments_.emplace_back(std::move(value));
	}

	size_t parameters::size() const
	{
		return this->arguments_.size();
	}

	const std::string& parameters::get(const size_t index) const
	{
		return this->arguments_.at(index);
	}

	std::string parameters::join(const size_t index, const std::string& separator) const
	{
		std::string buffer{};

		for (auto i = index; i < this->size(); ++i)
		{
			if (i != 0)
			{
				buffer.append(separator);
			}

			buffer.append(this->get(i));
		}

		return buffer;
	}

	const std::string& parameters::operator[](const size_t index) const
	{
		return this->get(index);
	}

	parameters::list_type::iterator parameters::begin()
	{
		return this->arguments_.begin();
	}

	parameters::list_type::const_iterator parameters::begin() const
	{
		return this->arguments_.begin();
	}

	parameters::list_type::iterator parameters::end()
	{
		return this->arguments_.end();
	}

	parameters::list_type::const_iterator parameters::end() const
	{
		return this->arguments_.end();
	}

	bool parameters::has(const std::string& value) const
	{
		for (const auto& val : this->arguments_)
		{
			if (val == value)
			{
				return true;
			}
		}

		return false;
	}
}
