#ifndef SWAN_UTIL_CIRCULARBUFFER_HPP
#define SWAN_UTIL_CIRCULARBUFFER_HPP

#include <array> // For std::array<T, Size>

namespace SWAN {
    namespace Util {
	template <typename T, unsigned Size>
	class CircularBuffer {
	public:
	    void push(const T& t) { arr[size++] = t; }

	    T& front() { return arr[topIndex]; }
	    T& back()  { return arr[(topIndex + 1) % 10]; }

	    Iterator begin() { return Iterator(arr, topIndex); }

	    class Iterator {
	    public:
		Iterator(const std::array<T, Size>& arr) : arr(arr) {}

		//TODO: FINISH THIS
		void operator++() const {}
		void operator--() const {}
		T& operator*() const {}
		T* operator->() const {}

		bool operator==(const Iterator& other) const {}
		bool operator!=(const Iterator& other) const {}

	    private:
		int index;
		const std::array<T, Size>& arr;
	    };

	private:
	    std::array<T, Size> arr;
	    int size = 0;
	};
    } // namespace Util
} // namespace SWAN

#endif
