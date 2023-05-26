#include <std_include.hpp>
#include "patch_kill_list_command.hpp"

#include "crypto_key.hpp"
#include "services/kill_list.hpp"

#include <utils/parameters.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

const char* patch_kill_list_command::get_command() const
{
	return "patchkill";
}

// patchkill timestamp signature add/remove target_ip (ban_reason)
void patch_kill_list_command::handle_command([[maybe_unused]] const network::address& target, const std::string_view& data)
{
	const utils::parameters params(data);
	if (params.size() < 3)
	{
		throw execution_exception("Invalid parameter count");
	}

	const auto supplied_timestamp = std::chrono::seconds(std::stoul(params[0]));
	const auto current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());

	// Abs the duration so that the client can be ahead or behind
	const auto time_stretch = std::chrono::abs(current_timestamp - supplied_timestamp);

	// not offset by more than 5 minutes in either direction
	if (time_stretch > 5min)
	{
		throw execution_exception(utils::string::va("Invalid timestamp supplied - expected %llu, got %llu, which is more than 5 minutes apart", current_timestamp.count(), supplied_timestamp.count()));
	}

	const auto& signature = utils::cryptography::base64::decode(params[1]);
	const auto should_remove = params[2] == "remove"s;

	if (!should_remove && params[2] != "add"s)
	{
		throw execution_exception("Invalid parameter #2: should be 'add' or 'remove'");
	}

	const auto supplied_reason = params.join(4);
	const auto& crypto_key = crypto_key::get(); 
	const auto signature_candidate = std::to_string(supplied_timestamp.count());

	if (!utils::cryptography::ecc::verify_message(crypto_key, signature_candidate, signature))
	{
		throw execution_exception("Signature verification of the kill list patch key failed");
	}

	const auto kill_list_service = this->get_server().get_service<kill_list>();
	const auto& supplied_address = params[3];

	if (should_remove)
	{
		kill_list_service->remove_from_kill_list(supplied_address);
	}
	else
	{
		kill_list_service->add_to_kill_list(kill_list::kill_list_entry(supplied_address, supplied_reason));
	}
}
