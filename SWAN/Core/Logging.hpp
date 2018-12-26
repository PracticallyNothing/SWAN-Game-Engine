#ifndef SWAN_LOGGING_HPP
#define SWAN_LOGGING_HPP

#include <string>
#include <sstream>
#include <set>
#include "Utility/StreamOps.hpp"

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

    void Log(std::string message, LogLevel lvl = LogLevel::Info);
    void Log(std::string category, std::string message, LogLevel lvl = LogLevel::Info);

    template<typename T>
    void LogVar(const std::string& varName, const T& t, LogLevel lvl = LogLevel::Info) {
	std::stringstream ss;
	ss << t;
	Log(varName + ": " + ss.str(), lvl);
    }
    template<typename T>
    void LogVar(const std::string& category, const std::string& varName, const T& t, LogLevel lvl = LogLevel::Info) {
	std::stringstream ss;
	ss << t;
	Log(category, varName + ": " + ss.str(), lvl);
    }

    namespace detail {
	extern LogLevel maxLogLevel;
	extern std::set<std::string> blacklistedCategories;
    }

    inline void FilterOutLogCategory(const std::string& category) { detail::blacklistedCategories.insert(category); }
    inline void SetLogLevel(LogLevel lvl) { detail::maxLogLevel = lvl; }
}

#endif
