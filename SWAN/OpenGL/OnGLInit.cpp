#include "OnGLInit.hpp"

namespace SWAN {
    namespace detail {
	bool glInit = false;

	/// Vector of functions to call upon GL initialization.
	std::vector<std::function<void(void)>> funcs;

	void finishInit() {
	    glInit = true;
	    for(auto& f : funcs) f();
	}
    }
}
