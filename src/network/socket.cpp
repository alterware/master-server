#include "std_include.hpp"

#include "network/socket.hpp"

namespace network
{
	namespace
	{
#ifdef _WIN32
		[[maybe_unused]] class wsa_initializer
		{
		public:
			wsa_initializer()
			{
				WSADATA wsa_data;
				if (WSAStartup(MAKEWORD(2, 2), &wsa_data))
				{
					throw std::runtime_error("Unable to initialize WSA");
				}
			}

			~wsa_initializer()
			{
				WSACleanup();
			}
		} _;
#endif
	}

	socket::socket()
	{
		this->socket_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

	socket::~socket()
	{
		if (this->socket_ != INVALID_SOCKET)
		{
#ifdef _WIN32
			closesocket(this->socket_);
#else
			close(this->socket_);
#endif
		}
	}

	socket::socket(socket&& obj) noexcept
	{
		this->operator=(std::move(obj));
	}

	socket& socket::operator=(socket&& obj) noexcept
	{
		if (this != &obj)
		{
			this->~socket();
			this->socket_ = obj.socket_;
			obj.socket_ = INVALID_SOCKET;
		}

		return *this;
	}

	bool socket::bind(const address& target)
	{
		return ::bind(this->socket_, &target.get_addr(), sizeof(target.get_addr())) == 0;
	}

	void socket::send(const address& target, const std::string& data) const
	{
		sendto(this->socket_, data.data(), static_cast<int>(data.size()), 0, &target.get_addr(),
		       sizeof(target.get_addr()));
	}

	bool socket::receive(address& source, std::string& data) const
	{
		char buffer[0x2000];
		socklen_t len = sizeof(source.get_in_addr());

		const auto result = recvfrom(this->socket_, buffer, sizeof(buffer), 0, &source.get_addr(), &len);
		if (result == SOCKET_ERROR) // Probably WSAEWOULDBLOCK
		{
			return false;
		}

		data.assign(buffer, buffer + result);
		return true;
	}

	bool socket::set_blocking(const bool blocking)
	{
#ifdef _WIN32
		unsigned long mode = blocking ? 0 : 1;
		return ioctlsocket(this->socket_, FIONBIO, &mode) == 0;
#else
		int flags = fcntl(this->socket_, F_GETFL, 0);
		if (flags == -1) return false;
		flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
		return fcntl(this->socket_, F_SETFL, flags) == 0;
#endif
	}

	bool socket::sleep(const std::chrono::milliseconds timeout) const
	{
		fd_set fdr;
		FD_ZERO(&fdr);
		FD_SET(this->socket_, &fdr);

		const auto msec = timeout.count();

		timeval tv{};
		tv.tv_sec = static_cast<long>(msec / 1000ll);
		tv.tv_usec = static_cast<long>((msec % 1000) * 1000);

		const auto retval = select(static_cast<int>(this->socket_) + 1, &fdr, nullptr, nullptr, &tv);
		if (retval == SOCKET_ERROR)
		{
			std::this_thread::sleep_for(1ms);
			return socket_is_ready;
		}

		if (retval > 0)
		{
			return socket_is_ready;
		}

		return !socket_is_ready;
	}
}
