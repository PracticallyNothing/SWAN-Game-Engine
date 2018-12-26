#ifndef OPENAL_DEBUGGING_HPP
#define OPENAL_DEBUGGING_HPP

#include <iostream>
#include <string>
#include <map>

#include <AL/al.h>

#define CONTEXT_STR std::string(__FILE__) + ":" + std::to_string(__LINE__) + " | " + __PRETTY_FUNCTION__
extern std::map<ALenum, std::string> errors;

#ifdef DEBUG
#    define AL(func) do { (func); std::cout << CONTEXT_STR << ": " << errors[alGetError()] << '\n'; } while (0)
#else
#    define AL(func) do { (func); } while (0)
#endif

#endif
