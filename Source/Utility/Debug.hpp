#ifndef UTILITY_DEBUG
#define UTILITY_DEBUG

# define STRINGIFY_IMPL(text)  	  #text
# define STRINGIFY(text)       	  STRINGIFY_IMPL(text)
# define CONCATENATE_IMPL(a, b)	  a##b
# define CONCATENATE(a, b)		  CONCATENATE_IMPL(a, b)

# ifdef NO_DEBUG
# error \
	Header "Utility/Debug.hpp" included, but NO_DEBUG is defined. \
	[Triggered by NO_DEBUG]
# endif

# if defined(DEBUG) && !defined(NO_DEBUG)
#	 define DEBUG_OUT(function)   	  do { std::cout << STRINGIFY(function) << ": " << (function) << '\n'; } while(0)
#    define DEBUG_PRINT_CONTEXT() 	  do { std::cout << "Context: " << __PRETTY_FUNCTION__ << '\n'; } while(0)
#    define DEBUG_VAR(type, name) 	  type name
#    define DEBUG_USING(as, type) 	  using as = type
#	 define DEBUG_DO(expr)		  	  do { (expr); } while(0)
#	 define DEBUG_IF(condition, expr) do { if(condition) (expr); } while(0)
#	 define DEBUG_IF_ELSE(condition, expr_true, expr_false) \
								do { 						\
									if(condition) 			\
										(expr_true); 		\
									else 					\
										(expr_false) 		\
								} while(0)
# else
#	 define DEBUG_OUT(function)   	  (void)0
#    define DEBUG_PRINT_CONTEXT() 	  (void)0
#	 define DEBUG_VAR(type, name) 	  struct CONCATENATE(CONCATENATE(__, __LINE__), __) {}
#    define DEBUG_USING(type, as) 	  struct CONCATENATE(__, __LINE__) {}
#	 define DEBUG_DO(func)		      (void)0
#	 define DEBUG_IF(condition, expr) (void)0
#	 define DEBUG_IF_ELSE(condition, expr_true, expr_false) (void)0
# 	 warning Header "Utility/Debug.hpp" included, but DEBUG isn't defined.
# endif

#endif
