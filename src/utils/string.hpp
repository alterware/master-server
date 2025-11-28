#pragma once
#include "memory.hpp"
#include <cstdint>

template <class Type, std::size_t n>
constexpr std::size_t ARRAY_COUNT(Type(&)[n]) { return n; }

namespace utils::string
{
	template <std::size_t buffers, std::size_t min_buffer_size>
	class va_provider final
	{
	public:
		static_assert(buffers != 0 && min_buffer_size != 0, "buffers and min_buffer_size mustn't be 0");

		va_provider() : current_buffer_(0)
		{
		}

		char* get(const char* format, va_list ap)
		{
			++this->current_buffer_ %= ARRAY_COUNT(this->string_pool_);
			auto entry = &this->string_pool_[this->current_buffer_];

			if (!entry->size_ || !entry->buffer_)
			{
				throw std::runtime_error("String pool not initialized");
			}

			while (true)
			{
				const auto res = vsnprintf(entry->buffer_, entry->size_, format, ap);
				if (res < 0) return nullptr; // Error

				if (static_cast<std::size_t>(res) >= entry->size_)
				{
					entry->double_size();
					continue;
				}

				break; // Success
			}

			return entry->buffer_;
		}

	private:
		class entry final
		{
		public:
			explicit entry(const std::size_t size = min_buffer_size) : size_(size), buffer_(nullptr)
			{
				if (this->size_ < min_buffer_size) this->size_ = min_buffer_size;
				this->allocate();
			}

			~entry()
			{
				if (this->buffer_) memory::get_allocator()->free(this->buffer_);
				this->size_ = 0;
				this->buffer_ = nullptr;
			}

			void allocate()
			{
				if (this->buffer_) memory::get_allocator()->free(this->buffer_);
				this->buffer_ = memory::get_allocator()->allocate_array<char>(this->size_ + 1);
			}

			void double_size()
			{
				this->size_ *= 2;
				this->allocate();
			}

			std::size_t size_;
			char* buffer_;
		};

		std::size_t current_buffer_;
		entry string_pool_[buffers];
	};

	const char* va(const char* fmt, ...);

	std::vector<std::string> split(const std::string& s, char delim);
	std::vector<std::string> split(const std::string_view& s, char delim);

	std::string to_lower(std::string text);
	std::string to_upper(std::string text);

	bool starts_with(const std::string& text, const std::string& substring);
	bool ends_with(const std::string& text, const std::string& substring);

	std::string dump_hex(const std::string& data, const std::string& separator = " ");

	std::string replace(std::string str, const std::string& from, const std::string& to);
}
