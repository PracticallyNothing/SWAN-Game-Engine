#ifndef UTIL_STRING_UTIL_HPP
#define UTIL_STRING_UTIL_HPP

#include <array>  // For std::array<T,N>
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "../Utility/FindNext.hpp" // For Util::FindNext<Iter, T>()
#include "../Utility/Collect.hpp"  // For Util::collect_iter()

namespace Util {
	using std::array;
	using std::string;
	using std::vector;
	
	string trim(string s);	
	
	vector<string> splitOn(const string& line, char c = ' ');

	template<typename Pred>
	vector<string> splitIf(const string& line, Pred&& p);
	
	array<int, 5> readFace(string str);

	string getDirectory(const string& location, bool withSlash = true);
}

#endif
