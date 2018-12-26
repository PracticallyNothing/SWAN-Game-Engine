#include "Logging.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <algorithm>

#include "Utility/StringUtil.hpp"

namespace SWAN
{
    namespace detail {
	LogLevel maxLogLevel = LogLevel::Debug;
	std::set<std::string> blacklistedCategories;
    }

    static constexpr const char* LogLevelToString(LogLevel level)
    {
	switch(level)
	{
	case LogLevel::Debug:   return "  DEBUG";
	case LogLevel::Info:    return "   INFO";
	case LogLevel::Success: return "SUCCESS";
	case LogLevel::Warning: return "WARNING";
	case LogLevel::Error:   return "  ERROR";
	case LogLevel::Fatal:   return "  FATAL";
	}
    }

    static std::string ColorLog(const std::string& str, LogLevel lvl)
    {
	switch(lvl)
	{
	case LogLevel::Debug:   return "\033[30;46m"   + str + "\033[0m";
	case LogLevel::Info:    return "\033[7m"       + str + "\033[0m";
	case LogLevel::Success: return "\033[30;42m"   + str + "\033[0m";
	case LogLevel::Warning: return "\033[30;43m"   + str + "\033[0m";
	case LogLevel::Error:   return "\033[37;41m"   + str + "\033[0m";
	case LogLevel::Fatal:   return "\033[1;37;41m" + str + "\033[0m";
	}
    }

    void Log(std::string message, LogLevel lvl)
    {
	if(lvl < detail::maxLogLevel)
	    return;


	std::cout
	    //<< std::setw(20)
	    << ColorLog(std::string("[") + LogLevelToString(lvl) + "]", lvl)
	    << ": " << message << '\n';

	if(lvl == LogLevel::Fatal) {
	    std::cout << ColorLog("[FATAL]:", LogLevel::Fatal) << " Exiting...\n";
	    std::exit(EXIT_FAILURE);
	}
    }

    void Log(std::string category, std::string message, LogLevel lvl)
    {
	if(lvl < detail::maxLogLevel)
	    return;

	auto v = Util::SplitOn(category, '|');
	for(const std::string& s : v)
	{
	    if(detail::blacklistedCategories.find(s) != detail::blacklistedCategories.end())
		return;
	}

	std::cout << ColorLog(std::string("[") + LogLevelToString(lvl) + "][" + category + "]", lvl)
		  << ": " << message << '\n';

	if(lvl == LogLevel::Fatal) {
	    std::cout << "  Exiting...\n";
	    std::exit(EXIT_FAILURE);
	}
    }
}
