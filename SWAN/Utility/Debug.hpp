#ifndef UTILITY_DEBUG
#define UTILITY_DEBUG

#define SWAN_STRINGIFY_IMPL(text) #text
#define SWAN_STRINGIFY(text) SWAN_STRINGIFY_IMPL(text)
#define SWAN_CONCATENATE_IMPL(a, b) a##b
#define SWAN_CONCATENATE(a, b) SWAN_CONCATENATE_IMPL(a, b)

#ifdef SWAN_NO_DEBUG
#	error Header "Utility/Debug.hpp" included, but SWAN_NO_DEBUG is defined. \
    [Triggered by SWAN_NO_DEBUG]
#endif

#include "Core/Logging.hpp"

#if defined(SWAN_DEBUG) && !defined(SWAN_NO_DEBUG)
#	include <iostream>
// using namespace SWAN::Util::StreamOps;

#	define SWAN_DEBUG_PRINT(str)                    \
		do {                                         \
			SWAN::Log((str), SWAN::LogLevel::Debug); \
		} while(0)
#	define SWAN_DEBUG_PRINT_CATEGORY(str, category)           \
		do {                                                   \
			SWAN::Log(category, (str), SWAN::LogLevel::Debug); \
		} while(0)
#	define SWAN_DEBUG_OUT(function)                                                                         \
		do {                                                                                                 \
			SWAN::LogVar<decltype((function))>(SWAN_STRINGIFY(function), (function), SWAN::LogLevel::Debug); \
		} while(0)
#	define SWAN_DEBUG_OUT_CATEGORY(function, category)                                                       \
		do {                                                                                                  \
			SWAN::LogVar(category, std::string(SWAN_STRINGIFY(function)), (function), SWAN::LogLevel::Debug); \
		} while(0)
#	define SWAN_DEBUG_PRINT_CONTEXT()                        \
		do {                                                  \
			std::cout << "Context: " << __FUNCTION__ << '\n'; \
		} while(0)
#	define SWAN_DEBUG_PRINT_CONTEXTUAL(str)                              \
		do {                                                              \
			std::cout << "[" << __FUNCTION__ << "()]: " << (str) << '\n'; \
		} while(0)
#	define SWAN_DEBUG_VAR(type, name) type name
#	define SWAN_DEBUG_USING(as, type) using as = type
#	define SWAN_DEBUG_DO(expr) \
		do {                    \
			(expr);             \
		} while(0)
#	define SWAN_DEBUG_IF(condition, expr) \
		do {                               \
			if(condition)                  \
				(expr);                    \
		} while(0)
#	define SWAN_DEBUG_IF_ELSE(condition, expr_true, expr_false) \
		do {                                                     \
			if(condition)                                        \
				(expr_true);                                     \
			else                                                 \
				(expr_false);                                    \
		} while(0)
#else
#	define SWAN_DEBUG_PRINT(str) (void) 0
#	define SWAN_DEBUG_PRINT_CATEGORY(str, category) (void) 0
#	define SWAN_DEBUG_OUT(function) (void) 0
#	define SWAN_DEBUG_OUT_CATEGORY(function, category) (void) 0
#	define SWAN_DEBUG_PRINT_CONTEXT() (void) 0
#	define SWAN_DEBUG_PRINT_CONTEXTUAL(str) (void) 0
#	define SWAN_DEBUG_VAR(type, name)                                \
		struct SWAN_CONCATENATE(SWAN_CONCATENATE(__, __LINE__), __) { \
		}
#	define SWAN_DEBUG_USING(type, as)          \
		struct SWAN_CONCATENATE(__, __LINE__) { \
		}
#	define SWAN_DEBUG_DO(func) (void) 0
#	define SWAN_DEBUG_IF(condition, expr) (void) 0
#	define SWAN_DEBUG_IF_ELSE(condition, expr_true, expr_false) (void) 0
//#warning Header "Utility/Debug.hpp" included, but SWAN_DEBUG isnt defined.
#endif

#endif
