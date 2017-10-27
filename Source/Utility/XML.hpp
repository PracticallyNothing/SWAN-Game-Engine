#ifndef SWAN_UTIL_XML_HPP
#define SWAN_UTIL_XML_HPP

#include <iostream> // For std::ostream
#include <map>      // For std::map<K, V>
#include <string>   // For std::string
#include <utility>  // For std::move()
#include <vector>   // For std::vector<T>

#include "Utility/Debug.hpp" // For SWAN_DEBUG_PRINT()

namespace SWAN {
namespace Util {
	struct XML;
	class XMLTag;

	class XMLTag {
	  public:
		XMLTag () {}

		~XMLTag () {
			//DEBUG_PRINT(std::string("        [XMLTag::~XMLTag()] Deleting ") + getFullName());

			for (auto c : children) {
				delete c;
			}
		}

		XMLTag (XMLTag&& other)
		  : name (std::move (other.name))
		  , data (std::move (other.data))
		  , attribs (std::move (other.attribs))
		  , children (std::move (other.children))
		  , parent (std::move (other.parent)) {
			other.attribs.clear ();
			other.children.clear ();
		}

		void operator= (XMLTag&& other) {
			name     = std::move (other.name);
			data     = std::move (other.data);
			attribs  = std::move (other.attribs);
			children = std::move (other.children);
			parent   = std::move (other.parent);

			other.attribs.clear ();
			other.children.clear ();
		}

		XMLTag (const XML& other) = delete;
		XMLTag& operator= (const XML& other) = delete;

		bool hasAttrib (const std::string& name) const { return attribs.find (name) != attribs.end (); }

		bool isRoot () const { return parent; }

		std::string getAttrib (const std::string& name) const { return attribs.find (name)->second; }

		std::string getFullName () const {
			if (parent)
				return parent->getFullName (name);
			else
				return name;
		}

		std::vector<const XMLTag*> findTagsWithName (std::string name) const;

		std::string name;
		std::string data;
		std::map<std::string /* name */, std::string /* value */> attribs;
		const XMLTag*        parent = nullptr;
		std::vector<XMLTag*> children;

	  private:
		std::string getFullName (const std::string& postfix) const {
			if (parent)
				return parent->getFullName (name + "::" + postfix);
			else
				return name + "::" + postfix;
		}

		void findTagsWithName (const std::string& name, std::vector<const XMLTag*>& v) const;
	};

	struct XML {
		bool        hasDeclTag = false;
		XMLTag      declTag;
		XMLTag      root;
		std::string filename;

		std::vector<const XMLTag*> findTagsWithName (const std::string& name) { return root.findTagsWithName (name); }
	};

	XML ReadXML (std::string filename);

	std::string Stringify (const XML& xml);
	std::string Stringify (const XMLTag& tag, int indent = 0);

	namespace StreamOps {
		inline std::ostream& operator<< (std::ostream& os, const XML& xml) {
			os << Stringify (xml);
			return os;
		}

		inline std::ostream& operator<< (std::ostream& os, const XMLTag& tag) {
			os << Stringify (tag);
			return os;
		}
	}
}
}

#endif
