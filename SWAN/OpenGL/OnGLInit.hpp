#ifndef SWAN_ON_GL_INIT
#define SWAN_ON_GL_INIT

#include <vector> // For std::vector<T>
#include <functional> // For std::function<Sig>
#include <initializer_list> // For std::initializer_list<T>

namespace SWAN {
    namespace detail {
	/// Is OpenGL initialized?
	extern bool glInit;

	/// Vector of functions to call upon GL initialization.
	extern std::vector<std::function<void(void)>> funcs;

	/// Finishes OpenGL initialization by calling all functions in funcs.
	/// Called by SWAN::Display::Init().
	extern void finishInit();
    }

    /// Is OpenGL initialized?
    /// @note To initialize an OpenGL context, use SWAN::Display::Init().
    inline bool IsGLInitialized() { return detail::glInit; }

    struct OnGLInit {
	/// Do nothing. NOP constructor.
	OnGLInit() {}

	/// Register a function to call on OpenGL initialization.
	/// @param f Function that receives no arguments,
	///          returns nothing and updates the environment.
	OnGLInit(std::function<void(void)> f) {
	    if(!IsGLInitialized())
		detail::funcs.push_back(f);
	    else
		f();
	}

	/// Register multiple functions to call on OpenGL initialization.
	/// @param i List of functions that receive no arguments,
	////         return nothing and update the environment.
	OnGLInit(std::initializer_list<std::function<void(void)>> i) {
	    if(!IsGLInitialized())
		detail::funcs.insert(detail::funcs.end(), i.begin(), i.end());
	    else
		for(auto f : i)
		    f();
	}
    };
}

#endif
