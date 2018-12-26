#error Framebuffer.hpp contains unfinished code. Don't use it.

#ifndef SWAN_GL_FRAMEBUFFER_HPP
#define SWAN_GL_FRAMEBUFFER_HPP

#include <glad/glad.h>

namespace SWAN {
    namespace GL {
	struct Framebuffer;

	namespace detail {
	    const extern Framebuffer* currBoundFB;
	}

	struct Framebuffer {
	    Framebuffer() {
		glGenFramebuffers(1, &id);
	    }
	    ~Framebuffer() {
		glDeleteFramebuffers(1, &id);
	    }

	    GLuint id = 0;
	};
    }
}

#endif
