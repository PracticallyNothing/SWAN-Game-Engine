#ifndef SWAN_CONSOLE_HPP
#define SWAN_CONSOLE_HPP

#include <sstream>
#include <string>
#include <vector>

#include "Utility/CircularBuffer.hpp" // For SWAN::Util::CircularBuffer<T, Size>

namespace SWAN {
namespace Console {
	namespace detail {
		SWAN::Util::CircularBuffer<std::string, 20> buffer;
	}
	void Print(char c);
	void Print(std::string s);

	inline void Print(const char* s, int len = -1) {
		if(len < 0)
			Print(std::string(s));
		else if(len > 0)
			Print(std::string(s, len));
	}

	inline void Print(const std::stringstream& s) { Print(ss.str()); }

	template <typename T>
	inline void PrintLine(T t) {
		Print(t);
		Print('\n');
	}
}
}

#endif
