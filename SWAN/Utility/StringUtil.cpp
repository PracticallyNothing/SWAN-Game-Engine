#include "StringUtil.hpp"

#include <sstream> // For std::stringstream
#include <algorithm> // For std::transform()
#include <cctype>  // For ::toupper()

namespace SWAN {
    namespace Util {
	String ToLower(const String& s) {
	    String res(s);
	    std::transform(res.begin(), res.end(), res.begin(), ::tolower);
	    return res;
	}

	String ToUpper(const String& s) {
	    String res(s);
	    std::transform(res.begin(), res.end(), res.begin(), ::toupper);
	    return res;
	}

	Array<String, 2> SplitOnFirst(const String& str, char delim){
	    Array<String, 2> res;

	    if(str.find_first_of(delim) == std::string::npos){ 
		res[0] = str;
		return res;
	    }

	    res[0] = str.substr(0, str.find_first_of(delim));
	    res[1] = str.substr(str.find_first_of(delim));

	    return res;
	}

	Array<String, 2> SplitOnLast(const String& str, char delim){
	    Array<String, 2> res;

	    if(str.find_last_of(delim) == std::string::npos){ 
		res[0] = str;
		return res;
	    }

	    res[0] = str.substr(0, str.find_last_of(delim));
	    res[1] = str.substr(str.find_last_of(delim));

	    return res;
	}

	String Trim(const String& s) {
	    String res(s);

	    if(isspace(res[0])) {
		size_t i;
		for(i = 0; i < res.length() && isspace(res[i]); i++); // Trim front
		res.erase(0, i);
	    }

	    if(isspace(res.back())) {
		int ii;
		for(ii = res.length() - 1; ii >= 0 && isspace(res[ii]); ii--); // Trim back
		if(isspace(res[ii+1]))
		    res.erase(ii+1);
	    }

	    return res;
	}

	Vector<String> SplitOn(const String& str, char delim) {
	    Vector<String> res;
	    res.reserve(str.length() / 4);

	    std::stringstream ss;
	    int prevI = 0;

	    for(unsigned i = 0; prevI + i <= str.length(); i++) {
		char c = str[i + prevI];
		if(c == delim || (prevI + i == str.length() && ss.str().length())) {
		    res.push_back(ss.str());
		    ss.str("");
		    prevI += i;
		    i = 0;
		} else {
		    ss << c;
		}
	    }

	    return res;
	}

	template <typename Pred>
	Vector<String> SplitIf(const String& str, Pred&& p) {
	    Vector<String> res;

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

	Array<int, 5> ReadFace(String str) {
	    Array<int, 5> res{ { 0, 0, 0, 0, 0 } };

	    str = Trim(str);
	    auto v = SplitOn(str, '/');

	    res[0] = stoi(v[0]);

	    if(v.size() > 1) res[1] = v[1].size();
	    if(res[1]) res[2] = stoi(v[1]);

	    if(v.size() > 2) res[3] = v[2].size();
	    if(res[3]) res[4] = stoi(v[2]);

	    return res;
	}

	String Unquote(const String& s) {
	    String res = Trim(s);

	    if(res.front() == '\"')
		res.erase(0, 1);

	    if(res.back() == '\"')
		res.pop_back();

	    return res;
	}

	String GetDirectory(const String& location, bool withSlash) {
	    auto i = location.find_last_of('/');

	    if(i == std::string::npos) {
		return (withSlash ? "./" : ".");
	    } else {
		return location.substr(0, i + (withSlash ? 1 : 0));
	    }
	}
	String GetFilename(const String& location, bool keepExtension){
	    auto i = location.find_last_of('/');

	    if(i == String::npos) {
		return keepExtension ? location : SplitOnLast(location, '.')[0];
	    } else {
		return keepExtension ? location.substr(i+1) : SplitOnLast(location.substr(i+1), '.')[0];
	    }
	}
    } // namespace Util
} // namespace SWAN
