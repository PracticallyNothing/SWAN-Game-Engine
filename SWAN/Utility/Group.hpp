#ifndef SWAN_UTILITY_GROUP_HPP
#define SWAN_UTILITY_GROUP_HPP

#include <array>

namespace SWAN {
    namespace Util {
	template <typename T, typename Iter>
	inline constexpr bool IsOneOf(T t, Iter begin, Iter end) {
	    while(begin != end) {
		if(t == *begin)
		    return true;
		begin++;
	    }
	    return false;
	}
    } // namespace Util
} // namespace SWAN

#endif
