#pragma once

#include <string>
#include <unordered_map>

namespace utils
{
	class parameters
	{
	public:
		using list_type = std::vector<std::string>;

		parameters() = default;
		parameters(std::string buffer);
		parameters(const std::string_view& buffer);

		void add(std::string value);

		size_t size() const;
		std::string join(size_t index = 0, const std::string& separator = " ") const;

		const std::string& operator [](size_t index) const;
		const std::string& get(size_t index) const;

		list_type::iterator begin();
		list_type::const_iterator begin() const;
		list_type::iterator end();
		list_type::const_iterator end() const;

		bool has(const std::string& value) const;

	private:
		list_type arguments_{};
	};
}
