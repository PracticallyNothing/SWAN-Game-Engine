#ifndef SWAN_UTIL_CXARRAY_HPP
#define SWAN_UTIL_CXARRAY_HPP

#include <algorithm>
#include <initializer_list>

namespace SWAN {
	namespace Util {
		template<typename T, int N>
		struct CxArray {
			constexpr CxArray() : arr() {}
			explicit constexpr CxArray(std::initializer_list<T> il) {
				for(auto it = il.begin(); it != il.end(); it++) {
					arr[std::distance(il.begin(), it)] = *it;
				}
			}

			constexpr unsigned size() const { return N; }
			constexpr T* data() { return arr; }
			constexpr const T* data() const { return arr; }

			constexpr T* begin() { return arr; }
			constexpr T* end  () { return arr + N; }
			constexpr const T* begin() const { return arr; }
			constexpr const T* end  () const { return arr + N; }

			constexpr T& get(unsigned index) { return arr[index]; }
			constexpr const T& get(unsigned index) const { return arr[index]; }

			constexpr T& operator[](unsigned index) { return arr[index]; }
			constexpr const T& operator[](unsigned index) const { return arr[index]; }
			T arr[N];
		};
	}
}

#endif
