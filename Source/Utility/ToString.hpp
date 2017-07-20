#ifndef MK2_UTIL_TOSTRING_HPP
#define MK2_UTIL_TOSTRING_HPP

#include <sstream>  // For std::stringstream, std::string

namespace Util {
    template <typename T1, typename... Args>
    std::string toString(const T1& first, Args&&... args) {
        std::stringstream ss;
        ss << first;
        return toString(args..., ss);
    }

    template <typename T1, typename... Args>
    std::string toString(const T1& first, Args&&... args, std::stringstream& ss) {
        ss << first;
        return toString(args..., ss);
    }

    template <typename T1>
    std::string toString(const T1& first, std::stringstream& ss) {
        return (ss << first).str();
    }

    std::string toString(std::stringstream& ss) { return ss.str(); }
}

#endif  // MK2_UTIL_HPP
