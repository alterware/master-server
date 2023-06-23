curl = {
	source = path.join(dependencies.basePath, "curl"),
}

function curl.import()
	links { "curl" }

	filter "toolset:msc*"
		links { "Crypt32.lib" }
	filter {}

	curl.includes()
end

function curl.includes()
	includedirs {
		path.join(curl.source, "include"),
	}

	defines {
		"CURL_STRICTER",
		"CURL_STATICLIB",
		"CURL_DISABLE_LDAP",
	}
end

function curl.project()
	project "curl"
		language "C"

		curl.includes()

		includedirs {
			path.join(curl.source, "lib"),
		}

		files {
			path.join(curl.source, "lib/**.c"),
			path.join(curl.source, "lib/**.h"),
		}

		defines {
			"BUILDING_LIBCURL",
		}

		filter "toolset:msc*"

		defines {
			"USE_SCHANNEL",
			"USE_WINDOWS_SSPI",
			"USE_THREADS_WIN32",
		}
		filter {}

		filter "toolset:not msc*"

			defines {
				"USE_GNUTLS",
				"USE_THREADS_POSIX",
			}

		filter {}

		warnings "Off"
		kind "StaticLib"
end

table.insert(dependencies, curl)
