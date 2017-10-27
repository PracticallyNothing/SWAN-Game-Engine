#include "StringUtil.hpp"

#include "Utility/Collect.hpp" // For SWAN::Util::CollectIter()

namespace SWAN {
namespace Util {
	string Trim(const string& s) {
		string res(s);

		size_t i;
		int ii;
		for(i = 0; i < res.length() && isspace(res[i]); i++)
			; // Trim front
		for(ii = res.length() - 1; ii >= 0 && isspace(res[ii]); ii--)
			; // Trim back

		res.erase(0, i);

		ii -= i;
		if(ii > 0 && isspace(res[ii]))
			res.erase(ii);

		return res;
	}

	// 17
	// 17/1/3
	// 17//3

	vector<string> SplitOn(const string& str, char c) {
		vector<string> res;
		int prevI = 0;
		for(int i = 0; prevI + i < str.length(); i++) {
			if(c == str[prevI + i]) {
				if(i == 0) {
					res.push_back("");
					prevI++;
				} else {
					res.push_back(str.substr(prevI, i));
				}
				prevI += i + 1;
				i = -1;
			}
		}

		res.push_back(str.substr(prevI));

		return res;
	}

	template <typename Pred>
	vector<string> SplitIf(const string& str, Pred&& p) {
		vector<string> res;

		size_t prevI = 0;
		for(size_t i = 0; prevI + i < str.length(); i++) {
			if(p(str[prevI + i])) {
				res.push_back(str.substr(prevI, i));
				prevI += i;
				i = 0;
			}
		}

		res.push_back(str.substr(prevI));

		return res;
	}

	array<int, 5> ReadFace(string str) {
		array<int, 5> res{ { 0, 0, 0, 0, 0 } };

		str    = Trim(str);
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

	string Unquote(const string& s) {
		string res = Trim(s);

		if(res.front() == '\"')
			res.erase(0, 1);

		if(res.back() == '\"')
			res.pop_back();

		return res;
	}

	string GetDirectory(const string& location, bool withSlash) {
		auto i = location.find_last_of('/');

		if(i == string::npos) {
			return (withSlash ? "./" : ".");
		} else {
			return location.substr(0, i + (withSlash ? 1 : 0));
		}
	}
}
}
