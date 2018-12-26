#ifndef SWAN_UTIL_FIND_EXTENDED_HPP
#define SWAN_UTIL_FIND_EXTENDED_HPP

namespace SWAN {
    namespace Util {
	/** @brief Finds the n'th occurence of an element in a sequence.
	 *
	 *  The Iter type must be inequality comparable (a != b),
	 *  incrementable (i++) and dereferencable (*i), returning a T.
	 *
	 *  The T type must be equality comparable (t1 == t2).
	 *
	 *  @param begin Beginning of range.
	 *  @param end End of range.
	 *  @param val Value to search for.
	 *  @param n Number of occurences to search for.
	 */
	template<typename Iter, typename T>
	inline Iter FindNth(Iter begin, Iter end, const T& val, int n) {
	    while(begin != end && n) {
		if(*begin == val) n--;
		if(n) begin++;
	    }
	    return begin;
	}
    }
}

#endif
