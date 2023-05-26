#include "std_include.hpp"
#include "crypto_key.hpp"
#include "console.hpp"

#include "utils/io.hpp"

namespace crypto_key
{
	namespace
	{
		bool load_key(utils::cryptography::ecc::key& key)
		{
			std::string data{};
			if (!utils::io::read_file("./private.key", &data))
			{
				return false;
			}

			key.deserialize(data);
			if (!key.is_valid())
			{
				console::warn("Loaded key is invalid!");
				return false;
			}

			return true;
		}

		utils::cryptography::ecc::key generate_key()
		{
			auto key = utils::cryptography::ecc::generate_key(512);
			if (!key.is_valid())
			{
				throw std::runtime_error("Failed to generate server key!");
			}

			if (!utils::io::write_file("./private.key", key.serialize()))
			{
				throw std::runtime_error("Failed to write server key!");
			}

			console::info("Generated cryptographic key: %llX", key.get_hash());
			return key;
		}

		utils::cryptography::ecc::key load_or_generate_key()
		{
			utils::cryptography::ecc::key key{};
			if (load_key(key))
			{
				console::log("Loaded cryptographic key: %llX", key.get_hash());
				return key;
			}

			return generate_key();
		}

		utils::cryptography::ecc::key get_key_internal()
		{
			auto key = load_or_generate_key();
			if (!utils::io::write_file("./public.key", key.get_public_key()))
			{
				console::error("Failed to write public key!");
			}
			return key;
		}
	}

	const utils::cryptography::ecc::key& get()
	{
		static auto key = get_key_internal();
		return key;
	}
}
