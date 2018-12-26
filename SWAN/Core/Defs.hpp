#ifndef SWAN_DEFS_HPP
#define SWAN_DEFS_HPP

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Utility/CxArray.hpp"

// #include <array>
// template<typename T, unsigned N> using Array = std::array<T, N>;

namespace SWAN {
    template<typename T, unsigned N> using Array = SWAN::Util::CxArray<T, N>;
    template<typename K, typename V> using Map = std::map<K,V>;
    template<typename T> using Vector = std::vector<T>;
    template<typename T> using Pointer = std::unique_ptr<T>;
    using String = std::string;
}

#endif
