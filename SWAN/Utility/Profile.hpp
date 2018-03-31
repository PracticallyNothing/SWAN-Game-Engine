#ifndef SWAN_UTILITY_PROFILE_HPP
#define SWAN_UTILITY_PROFILE_HPP

#include <SDL2/SDL.h> // For SDL_GetTicks()
#include <cstdint>    // For std::uint32_t
#include <string>     // For std::string

#include "OnScopeExit.hpp" // For SWAN::Util::OnScopeExit

#define SWAN_UTIL_GET_PROFILE() SWAN::Util::CurrentProfile

#ifdef SWAN_NO_PROFILE
#error Header "Utility/Profile.hpp" included, but SWAN_NO_PROFILE is defined. \
	[Triggered by SWAN_NO_PROFILE]
#endif

#if defined(SWAN_PROFILE) && !defined(SWAN_NO_PROFILE)
#define SWAN_UTIL_PROFILE()                                                 \
	SWAN::Util::CurrentProfile =                                            \
	    SWAN::Util::Profile{ __FUNCTION__, SDL_GetTicks() };                \
	SWAN::Util::OnScopeExit _([] {                                          \
		std::cout << SWAN::Util::CurrentProfile.funcName << ": "            \
		          << SDL_GetTicks() - SWAN::Util::CurrentProfile.durationMs \
		          << '\n';                                                  \
	});                                                                     \
	do {                                                                    \
	} while(0)
#else
#define SWAN_UTIL_PROFILE() (void) 0
//#warning Header "Utility/Profile.hpp" included, but SWAN_PROFILE isnt defined.
#endif

namespace SWAN {
namespace Util {
	struct Profile {
		const char* funcName;
		std::uint32_t durationMs;
	};

	static Profile CurrentProfile = Profile{ "", 0 };
}
}

#endif
