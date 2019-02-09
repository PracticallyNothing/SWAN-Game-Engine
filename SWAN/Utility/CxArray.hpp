#ifndef SWAN_UTIL_CXARRAY_HPP
#define SWAN_UTIL_CXARRAY_HPP

#include <algorithm>
#include <initializer_list>

namespace SWAN
{
	namespace Util
	{
		template <typename T, int N>
		/// Constexpr replacement for std::array
		struct CxArray {
			constexpr CxArray() : arr() {}
			explicit constexpr CxArray(std::initializer_list<T> il)
			{
				for(auto it = il.begin(); it != il.end(); it++) {
					arr[std::distance(il.begin(), it)] = *it;
				}
			}

			constexpr CxArray& operator=(std::initializer_list<T> il)
			{
				for(auto it = il.begin(); it != il.end(); it++) {
					arr[std::distance(il.begin(), it)] = *it;
				}
			}

			// ------ The data ------- //
			T arr[N];
			// ----------------------- //

			constexpr bool empty() const { return N; }
			constexpr unsigned size() const { return N; }
			constexpr unsigned max_size() const { return N; }

			constexpr T* data() { return arr; }

			constexpr T* begin() { return arr; }
			constexpr T* end() { return arr + N; }

			constexpr T& front() { return *begin(); }
			constexpr T& back() { return *end(); }

			constexpr T& get(unsigned index) { return arr[index]; }
			constexpr T& at(unsigned index)
			{
				static_assert(index > 0 && index < N, "SWAN::Util::CxArray::at() called with out of range index.");
				return arr[index];
			}
			constexpr T& operator[](unsigned index) { return arr[index]; }

			constexpr T& operator*() { return *arr; }
			constexpr operator T*() { return arr; }

			// --- Const versions of functions above. ---
			constexpr const T* data() const { return arr; }
			constexpr const T* begin() const { return arr; }
			constexpr const T* end() const { return arr + N; }
			constexpr const T& front() const { return *begin(); }
			constexpr const T& back() const { return *end(); }
			constexpr const T& at(unsigned index) const
			{
				static_assert(index > 0 && index < N, "Const SWAN::Util::CxArray::at() called with out of range index.");
				return arr[index];
			}
			constexpr const T& get(unsigned index) const { return arr[index]; }
			constexpr const T& operator[](unsigned index) const { return arr[index]; }

			constexpr const T& operator*() const { return *arr; }
			constexpr operator const T*() const { return arr; }
		};
	} // namespace Util
} // namespace SWAN

#endif
