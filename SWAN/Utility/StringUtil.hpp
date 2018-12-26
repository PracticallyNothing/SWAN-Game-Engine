#ifndef SWAN_UTIL_STRING_UTIL_HPP
#define SWAN_UTIL_STRING_UTIL_HPP

#include "Core/Defs.hpp"

namespace SWAN {
    namespace Util {
	String ToLower(const String& s);
	String ToUpper(const String& s);

	String Trim(const String& s);
	String Unquote(const String& s);

	Vector<String> SplitOn(const String& line, char c = ' ');
	Array<String, 2> SplitOnFirst(const String& line, char c = ' ');
	Array<String, 2> SplitOnLast(const String& line, char c = ' ');

	template <typename Pred>
	Vector<String> SplitIf(const String& line, Pred&& p);

	Array<int, 5> ReadFace(String str);

	String GetFilename(const String& location, bool keepExtension);
	String GetDirectory(const String& location, bool withSlash = true);

	inline bool IsAbsolutePath(const String& path) { return path[0] == '/'; }
	inline bool IsRelativePath(const String& path) { return path[0] != '/'; }

    } // namespace Util
} // namespace SWAN

#endif
