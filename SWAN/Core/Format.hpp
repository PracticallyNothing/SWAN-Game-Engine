#ifndef SWAN_FORMAT_HPP
#define SWAN_FORMAT_HPP

#include "Defs.hpp"
#include "Utility/StreamOps.hpp"

namespace SWAN
{
	inline String Format(const String& s) { return s; }

	/**
     *
     *
     */
	template <typename First, typename... Args>
	String Format(const String& fmtStr, First f, Args... args)
	{
		using namespace SWAN::Util::StreamOps;
		std::stringstream ss;

		auto fmt = fmtStr.begin();
		while(fmt != fmtStr.end()) {
			if(*fmt == '{') {
				/// Information about the conversion.
				std::stringstream info;
				while(*fmt != '}') {
					++fmt;
					info << *fmt;
				}
				++fmt;

				ss << f;
				ss << fmtStr.substr(std::distance(fmtStr.begin(), fmt));
				return Format(ss.str(), args...);
			} else {
				ss << *fmt;
				++fmt;
			}
		}

		return ss.str();
	}
} // namespace SWAN

#endif
