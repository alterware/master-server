#include "std_include.hpp"

#include "network/address.hpp"

namespace network
{
	address::address()
	{
		ZeroMemory(&this->address_, sizeof(this->address_));
	}

	address::address(const std::string& addr)
		: address()
	{
		this->parse(addr);
	}

	address::address(sockaddr_in& addr)
	{
		this->address_ = addr;
	}

	bool address::operator==(const address& obj) const
	{
		//return !std::memcmp(&this->address_, &obj.address_, sizeof(this->address_));
		return this->address_.sin_family == obj.address_.sin_family //
			&& this->address_.sin_addr.s_addr == obj.address_.sin_addr.s_addr //
			&& this->address_.sin_port == obj.address_.sin_port;
	}

	void address::set_ipv4(const in_addr addr)
	{
		this->address_.sin_family = AF_INET;
		this->address_.sin_addr = addr;
	}

	void address::set_port(const unsigned short port)
	{
		this->address_.sin_port = htons(port);
	}

	unsigned short address::get_port() const
	{
		return ntohs(this->address_.sin_port);
	}

	std::string address::to_string(bool with_port) const
	{
		char buffer[1000] = {0};
		inet_ntop(this->address_.sin_family, &this->address_.sin_addr, buffer, sizeof(buffer));

		auto address = std::string(buffer);
		if (with_port)
		{
			address += ":"s + std::to_string(this->get_port());
		}

		return address;
	}

	bool address::is_local() const
	{
		// According to: https://en.wikipedia.org/wiki/Private_network

		uint8_t bytes[4];
		*reinterpret_cast<uint32_t*>(&bytes) = this->address_.sin_addr.s_addr;

		// 10.X.X.X
		if (bytes[0] == 10)
		{
			return true;
		}

		// 192.168.X.X
		if (bytes[0] == 192
			&& bytes[1] == 168)
		{
			return true;
		}

		// 172.16.X.X - 172.31.X.X
		if (bytes[0] == 172
			&& bytes[1] >= 16
			&& bytes[1] < 32)
		{
			return true;
		}

		// 127.0.0.1
		if (this->address_.sin_addr.s_addr == 0x0100007F)
		{
			return true;
		}

		return false;
	}

	sockaddr& address::get_addr()
	{
		return reinterpret_cast<sockaddr&>(this->get_in_addr());
	}

	const sockaddr& address::get_addr() const
	{
		return reinterpret_cast<const sockaddr&>(this->get_in_addr());
	}

	sockaddr_in& address::get_in_addr()
	{
		return this->address_;
	}

	const sockaddr_in& address::get_in_addr() const
	{
		return this->address_;
	}

	void address::parse(std::string addr)
	{
		const auto pos = addr.find_last_of(':');
		if (pos != std::string::npos)
		{
			auto port = addr.substr(pos + 1);
			this->set_port(uint16_t(atoi(port.data())));

			addr = addr.substr(0, pos);
		}

		this->resolve(addr);
	}

	void address::resolve(const std::string& hostname)
	{
		addrinfo* result = nullptr;
		if (!getaddrinfo(hostname.data(), nullptr, nullptr, &result))
		{
			for (auto* i = result; i; i = i->ai_next)
			{
				if (i->ai_addr->sa_family == AF_INET)
				{
					const auto port = this->get_port();
					std::memcpy(&this->address_, i->ai_addr, sizeof(this->address_));
					this->set_port(port);

					break;
				}
			}

			freeaddrinfo(result);
		}
	}
}

std::size_t std::hash<network::address>::operator()(const network::address& a) const noexcept
{
	const auto h1 = std::hash<decltype(a.get_in_addr().sin_family)>{}(a.get_in_addr().sin_family);
	const auto h2 = std::hash<decltype(a.get_in_addr().sin_addr.s_addr)>{}(a.get_in_addr().sin_addr.s_addr);
	const auto h3 = std::hash<decltype(a.get_in_addr().sin_port)>{}(a.get_in_addr().sin_port);
	return h1 ^ (h2 << 1) ^ (h3 << 2);
}
