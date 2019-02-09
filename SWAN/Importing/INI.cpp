#include "INI.hpp"

#include "Core/Defs.hpp"
#include "Utility/StringUtil.hpp" // For SWAN::Util::Trim(), SWAN::Util::ToLower()

#include <algorithm> // For std::all_of()
#include <fstream>   // For std::ifstream

namespace SWAN
{
	namespace INI
	{
		bool IsBool(String val)
		{
			val = Util::ToLower(Util::Trim(val));
			return val == "true" || val == "on" || val == "off" || IsInt(val);
		}
		bool IsInt(String val)
		{
			val = Util::Trim(val);
			return std::all_of(val.begin(), val.end(), [](char c) { return std::isdigit(c); });
		}
		bool IsReal(String val)
		{
			val = Util::Trim(val);
			return std::all_of(val.begin(), val.end(),
			                   [dotFound = false](char c) mutable -> bool {
				                   if(c == '.' || c == ',') {
					                   if(!dotFound) {
						                   dotFound = true;
						                   return true;
					                   } else {
						                   return false;
					                   }
				                   }

				                   return std::isdigit(c);
			                   });
		}

		bool ToBool(String val)
		{
			val = Util::Trim(Util::ToLower(val));
			return val == "true" || val == "on" || val != "0";
		}

		int ToInt(String val) { return std::stoi(Util::Trim(val)); }
		float ToFloat(String val) { return std::stof(Util::Trim(val)); }
		double ToDouble(String val) { return std::stod(Util::Trim(val)); }

		Vector<String> ToArray(std::string val)
		{
			auto res = Util::SplitOn(val, ',');
			std::transform(res.begin(), res.end(), res.begin(), Util::Trim);
			res.resize(std::distance(res.begin(),
			                         std::remove_if(res.begin(), res.end(),
			                                        [](const std::string& s) { return s.length() == 0; })));
			return res;
		}

		Config ParseFile(const String& filename)
		{
			Config res;
			Section* currSec = &res["global"];

			std::ifstream file(filename);

			//size_t lineNum = 0;
			String line;

			while(std::getline(file, line)) {
				//lineNum++;
				line = Util::Trim(line);

				if(line[0] == '#' || line.length() == 0)
					continue;
				else if(line.length() < 3)
					continue;
				else if(line[0] != '[' && line.find('=') == std::string::npos)
					continue;

				// Extract text without comments at back of line

				// FIXIT: This is stupid, because a # should be able to appear in strings.
				//        Should instead first figure out type of line,
				//        then separate and ONLY THEN kill off comments at EOL.
				// Current solution: Just disallow comments on same line as non-comments
				//line = Util::Trim(Util::SplitOnFirst(line, '#')[0]);

				if(line[0] == '[') {
					String section = line.substr(1, line.find_first_of(']') - 1);
					if(!res.hasSection(section))
						res[section].name = section;

					currSec = &res[section];

				} else {
					auto v = Util::SplitOn(line, '=');
					if(v.size() > 2)
						continue;
					else if(v.size() < 2)
						continue;

					String name = Util::Unquote(Util::Trim(v[0]));
					String value = Util::Unquote(Util::Trim(v[1]));

					if(name.length() == 0)
						continue;
					else if(value.length() == 0)
						continue;

					currSec->insert({ name, value });
				}
			}

			return res;
		}
	} // namespace INI
} // namespace SWAN
