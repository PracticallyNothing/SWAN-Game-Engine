#ifndef SWAN_UTIL_INI_HPP
#define SWAN_UTIL_INI_HPP

#include <algorithm> // For std::all_of()
#include <map>       // For std::map<K, V>
#include <vector>    // For std::vector<T>

namespace SWAN {
    namespace Util {
	namespace INI {
	    struct Section : public std::map<std::string, std::string>{
		using Super = std::map<std::string, std::string>;

		std::string name;
		bool has(const std::string& name) const { return Super::find(name) != Super::end(); }
	    };
	    struct Config : public std::map<std::string, Section> {
		using Super = std::map<std::string, Section>;

		Config() { Super::insert({"global", Section()}); }

		Section& getGlobalSection() { return at("global"); }
		const Section& getGlobalSection() const { return at("global"); }

		bool hasSection(const std::string& name) const { return Super::find(name) != Super::end(); }
		bool hasVarInSection(const std::string& var, const std::string& section = "global") const {
		    return Super::find(section) != Super::end()
			&& Super::find(section)->second.has(var);
		}
		bool hasVar(const std::string& var) const {
		    for(Super::const_iterator it = begin(); it != end(); ++it){
			if(it->second.has(var))
			    return true;
		    }
		    return false;
		}

		void insert(const Section& s) { Super::insert({s.name, s}); }
	    };

	    bool IsBool(std::string val);
	    bool IsInt (std::string val);
	    bool IsReal(std::string val);

	    bool   ToBool  (std::string val);
	    int    ToInt   (std::string val);
	    float  ToFloat (std::string val);
	    double ToDouble(std::string val);

	    std::vector<std::string> ToArray(std::string val);

	    Config ParseFile(const std::string& filename);
	}
    }
}

#endif
