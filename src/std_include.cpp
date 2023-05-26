#include <std_include.hpp>

extern "C"
{
	int s_read_arc4random(void*, size_t)
	{
		return -1;
	}

#if !defined(__linux__)
	int s_read_getrandom(void*, size_t)
	{
		return -1;
	}
#endif

#ifdef _WIN32
	int s_read_urandom(void*, size_t)
	{
		return -1;
	}

	int s_read_ltm_rng(void*, size_t)
	{
		return -1;
	}
#else
	int s_read_wincsp(void*, size_t)
	{
		return -1;
	}
#endif
}
