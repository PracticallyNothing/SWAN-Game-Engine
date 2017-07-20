#ifndef UTIL_FIND_NEXT_HPP
#define UTIL_FIND_NEXT_HPP

namespace Util {
	template<typename InIter, typename T>
	struct FindNext {
		FindNext(InIter begin, InIter end, const T& val) 
			: begin(begin), end(end), val(val) {}
		
		InIter operator()(){
			while(begin < end){
				if(*begin == val){
					return begin++;
				}
				begin++;
			}
		}
		bool done(){ return !(begin < end); }

		InIter begin, end;
		const T& val;
	};
}

#endif
