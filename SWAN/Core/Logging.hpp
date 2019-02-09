#ifndef SWAN_LOGGING_HPP
#define SWAN_LOGGING_HPP

#include "Defs.hpp"
#include "Utility/StreamOps.hpp"

#include <set>
#include <sstream>
#include <string>

namespace SWAN
{
	using namespace SWAN::Util::StreamOps;
	enum class LogLevel : unsigned int {
		Debug = 0,
		Info,
		Success,
		Warning,
		Error,
		Fatal,
	};

	void Log(String message, LogLevel lvl = LogLevel::Info);
	void Log(String category, String message, LogLevel lvl = LogLevel::Info);

	template <typename T>
	void LogVar(const String& varName, const T& t, LogLevel lvl = LogLevel::Info)
	{
		std::stringstream ss;
		ss << t;
		Log(varName + ": " + ss.str(), lvl);
	}
	template <typename T>
	void LogVar(const String& category, const String& varName, const T& t, LogLevel lvl = LogLevel::Info)
	{
		std::stringstream ss;
		ss << t;
		Log(category, varName + ": " + ss.str(), lvl);
	}

	namespace detail
	{
		extern LogLevel maxLogLevel;
		extern std::set<String> blacklistedCategories;
	} // namespace detail

	inline void FilterOutLogCategory(const String& category) { detail::blacklistedCategories.insert(category); }
	inline void SetLogLevel(LogLevel lvl) { detail::maxLogLevel = lvl; }
} // namespace SWAN

#endif
