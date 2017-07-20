#ifndef MK2_UTIL_TOVECTOR_HPP
#define MK2_UTIL_TOVECTOR_HPP

#include <initializer_list>  // For std::initializer_list<T>
#include <vector>            // For std::vector<T>

namespace Util {
    template <typename T>
    std::vector<T> toVector(const std::initializer_list<T> il) {
        std::vector<T> res;

        for (auto& elem : il) {
            res.push_back(elem);
        }
        return res;
    }
}

#endif  // MK2_UTIL_HPP
