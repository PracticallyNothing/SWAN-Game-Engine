#ifndef SWAN_DEFS_HPP
#define SWAN_DEFS_HPP

#include "Utility/CxArray.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace SWAN
{
	/// Typedef for a constant size array. Internally uses SWAN::Util::CxArray<T,N>.
	template <typename T, unsigned N>
	using Array = SWAN::Util::CxArray<T, N>;

	/// Typedef for a type which maps keys to values. Internally uses std::map<K, V>
	template <typename K, typename V>
	using Map = std::map<K, V>;

	/// Typedef for a dynamic size array. Internally uses std::vector<T>.
	template <typename T>
	using Vector = std::vector<T>;

	/// Typedef for an owning pointer type. Internally uses std::unique_ptr<T>.
	template <typename T>
	using Pointer = std::unique_ptr<T>;

	/// Typedef for a string. Internally uses std::string.
	using String = std::string;
} // namespace SWAN

#endif
