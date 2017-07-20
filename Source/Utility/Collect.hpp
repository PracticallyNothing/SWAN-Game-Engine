#ifndef UTILITY_COLLECT_HPP
#define UTILITY_COLLECT_HPP

#include <vector> // For std::vector<T>

namespace Util {
    template<typename IterT, typename EqComp>
    std::vector<EqComp> collect(IterT begin, IterT end, EqComp value){
        std::vector<EqComp> result;

        while(begin != end){
            if(*begin == value) result.push_back(*begin);
            begin++;
        }

        return result;
    }

    template<typename IterT, typename Predicate>
    std::vector<IterT> collect_if(IterT begin, IterT end, Predicate&& p){
        std::vector<IterT> result;

        while(begin != end){
            if(p(*begin)) result.push_back(*begin);
            begin++;
        }

        return result;
    }

    template<typename IterT, typename EqComp>
    std::vector<IterT> collect_iter(IterT begin, IterT end, EqComp value){
        std::vector<IterT> result;

        while(begin != end){
            if(*begin == value) result.push_back(begin);
            begin++;
        }

        return result;
    }

    template<typename IterT, typename Predicate>
    std::vector<IterT> collect_iter_if(IterT begin, IterT end, Predicate&& p){
        std::vector<IterT> result;

        while(begin != end){
            if(p(*begin)) result.push_back(begin);
            begin++;
        }

        return result;
    }

} /* Util */

#endif
