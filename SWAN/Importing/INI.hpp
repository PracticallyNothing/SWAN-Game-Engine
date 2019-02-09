#ifndef SWAN_INI_HPP
#define SWAN_INI_HPP

#include "Core/Defs.hpp"
#include <map> // For Map<K, V>

namespace SWAN
{
	namespace INI
	{
		struct Section : public Map<String, String> {
			using Super = Map<String, String>;

			String name;
			bool has(const String& name) const { return Super::find(name) != Super::end(); }
		};

		struct Config : public Map<String, Section> {
			using Super = Map<String, Section>;

			Config() { Super::insert({ "global", Section() }); }

			Section& getGlobalSection() { return at("global"); }
			const Section& getGlobalSection() const { return at("global"); }

			bool hasSection(const String& name) const { return Super::find(name) != Super::end(); }
			bool hasVarInSection(const String& var, const String& section = "global") const
			{
				return Super::find(section) != Super::end()
				       && Super::find(section)->second.has(var);
			}

			// Does this config have this var anywhere at all?
			bool hasVar(const String& var) const
			{
				for(Super::const_iterator it = begin(); it != end(); ++it) {
					if(it->second.has(var))
						return true;
				}
				return false;
			}

			void insert(const Section& s) { Super::insert({ s.name, s }); }
		};

		bool IsBool(String val);
		bool IsInt(String val);
		bool IsReal(String val);

		bool ToBool(String val);
		int ToInt(String val);
		float ToFloat(String val);
		double ToDouble(String val);

		Vector<String> ToArray(String val);

		Config ParseFile(const String& filename);
	} // namespace INI
} // namespace SWAN

#endif
