#ifndef UTIL_XML_HPP
#define UTIL_XML_HPP

#include <vector> // For std::vector<T>
#include <string> // For std::string
#include <map>    // For std::map<K, V>

namespace Util {
	struct XML;
	class XMLTag;

	class XMLTag {
		public:
			~XMLTag(){
				for(auto c : children){
					delete c;
				}
			}

			std::string name;
			std::string data;
			std::map<std::string /* name */, std::string /* value */> attribs;
			bool hasAttrib(const std::string& name){ return attribs.find(name) != attribs.end(); }

			std::vector<const XMLTag*> findTagsWithName(std::string name);

			std::vector<XMLTag*> children;
		private:
			 void findTagsWithName(const std::string& name, std::vector<const XMLTag*>& v);
	};

	struct XML {
		bool hasDeclTag = false;
		XMLTag declTag;
		XMLTag root;
		std::string filename;

		std::vector<const XMLTag*> findTagsWithName(const std::string& name){ root.findTagsWithName(name); }
	};

	XML ReadXML(std::string filename);
}

#endif
