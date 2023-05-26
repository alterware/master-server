#pragma once

#include "server_base.hpp"
#include "server_list.hpp"
#include "client_list.hpp"
#include "service.hpp"

class server : public server_base
{
public:
	server(const network::address& bind_addr);

	server_list& get_server_list();
	const server_list& get_server_list() const;

	client_list& get_client_list();
	const client_list& get_client_list() const;

	template <typename T>
	T* get_service()
	{
		static_assert(std::is_base_of_v<service, T>, "Type must be a service!");

		for (auto& service : this->services_)
		{
			const auto& service_ref = *service;
			if (typeid(service_ref) == typeid(T))
			{
				return reinterpret_cast<T*>(service.get());
			}
		}

		return nullptr;
	}

private:
	server_list server_list_;
	client_list client_list_;

	std::vector<std::unique_ptr<service>> services_;
	std::unordered_map<std::string, service*> command_services_;

	template <typename T, typename... Args>
	void register_service(Args&&... args)
	{
		static_assert(std::is_base_of_v<service, T>, "Type must be a service!");

		auto service = std::make_unique<T>(*this, std::forward<Args>(args)...);
		auto* const command = service->get_command();
		if (command)
		{
			command_services_[command] = service.get();
		}

		services_.emplace_back(std::move(service));
	}

	void run_frame() override;
	void handle_command(const network::address& target, const std::string_view& command,
	                    const std::string_view& data) override;
};
