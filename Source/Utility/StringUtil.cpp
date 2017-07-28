#include "StringUtil.hpp"

#include "../Utility/Collect.hpp"  // For Util::CollectIter()

namespace Util {
	string Trim(string s) {
		size_t i;
		int ii;
		for (i = 0; i < s.length() && isspace(s[i]); i++);  // Trim front
		for (ii = s.length() - 1; ii >= 0 && isspace(s[ii]); ii--);  // Trim back

		s.erase(0, i);

		ii -= i;
		if (ii > 0 && isspace(s[ii])) s.erase(ii);

		return s;
	}
	vector<string> SplitOn(const string& line, char c) {
		vector<string> lineContents;

		size_t prevI = 0;
		for (size_t i = 0; prevI + i < line.length(); i++) {
			if (c == line[prevI + i]) {
				lineContents.push_back(line.substr(prevI, i));
				prevI += i+1;
				i = 0;
			}
		}

		lineContents.push_back(line.substr(prevI));

		return lineContents;
	}

	template<typename Pred>
	vector<string> SplitIf(const string& line, Pred&& p) {
		vector<string> lineContents;

		size_t prevI = 0;
		for (size_t i = 0; prevI + i < line.length(); i++) {
			if (p(line[prevI + i])) {
				lineContents.push_back(line.substr(prevI, i));
				prevI += i;
				i = 0;
			}
		}

		lineContents.push_back(line.substr(prevI));

		return lineContents;
	}
	array<int, 5> ReadFace(string str) {
		array<int, 5> res {{0, 0, 0, 0, 0}};

		str = Trim(str);
		auto v = SplitOn(str, '/');

		res[0] = stoi(v[0]);

		if(v.size() > 1)
			res[1] = v[1].size();
		if(res[1])
			res[2] = stoi(v[1]);

		if(v.size() > 2)
			res[3] = v[2].size();
		if(res[3])
			res[4] = stoi(v[2]);

		return res;
	}

	string GetDirectory(const string& location, bool withSlash){
		auto i = location.find_last_of('/');

		if(i == string::npos){
			return (withSlash ? "./" : ".");
		} else {
			return location.substr(0, i + (withSlash ? 1 : 0));
		}
	}
}
