#ifndef SWAN_UTILITY_COLLECT_HPP
#define SWAN_UTILITY_COLLECT_HPP

#include <vector> // For std::vector<T>

namespace SWAN {
    namespace Util {
	template <typename IterT, typename EqComp>
	std::vector<EqComp> Collect(IterT begin, IterT end, EqComp value) {
	    std::vector<EqComp> result;

	    while(begin != end) {
		if(*begin == value)
		    result.push_back(*begin);
		begin++;
	    }

	    return result;
	}

	template <typename IterT, typename Predicate>
	std::vector<IterT> CollectIf(IterT begin, IterT end, Predicate&& p) {
	    std::vector<IterT> result;

	    while(begin != end) {
		if(p(*begin))
		    result.push_back(*begin);
		begin++;
	    }

	    return result;
	}

	template <typename IterT, typename EqComp>
	std::vector<IterT> CollectIter(IterT begin, IterT end, EqComp value) {
	    std::vector<IterT> result;

	    while(begin != end) {
		if(*begin == value)
		    result.push_back(begin);
		begin++;
	    }

	    return result;
	}

	template <typename IterT, typename Predicate>
	std::vector<IterT> CollectIterIf(IterT begin, IterT end, Predicate&& p) {
	    std::vector<IterT> result;

	    while(begin != end) {
		if(p(*begin))
		    result.push_back(begin);
		begin++;
	    }

	    return result;
	}
    } // namespace Util
} // namespace SWAN

#endif
