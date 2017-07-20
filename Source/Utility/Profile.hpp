#ifndef UTILITY_PROFILE_HPP
#define UTILITY_PROFILE_HPP

#include <cstdint> 	  // For std::uint32_t
#include <string>  	  // For std::string
#include <SDL2/SDL.h> // For SDL_GetTicks()

#include "OnScopeExit.hpp" // For Util::OnScopeExit

#define UTIL_PROFILE() \
			Util::CurrentProfile = Util::Profile{ __PRETTY_FUNCTION__, SDL_GetTicks() }; \
			Util::OnScopeExit _([]{ std::cout << Util::CurrentProfile.funcName << ": " << SDL_GetTicks() - Util::CurrentProfile.durationMs << '\n'; }); \
			do {} while(0)

#define UTIL_GET_PROFILE() Util::CurrentProfile

namespace Util {
	struct Profile {
		const char* funcName;
		std::uint32_t durationMs;
	};

	static Profile CurrentProfile = Profile{"", 0};
}

#endif
