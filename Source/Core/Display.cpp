#include "Display.hpp"
#include "Utility/Debug.hpp"
#include <SDL2/SDL.h>

// #include <GL/glew.h>
#include <glad/glad.h>

namespace SWAN {
namespace Display {
	namespace detail {
		SDL_Window*   window;
		SDL_GLContext glContext;

		std::string title;
		int         width = -1, height = -1;
		bool        initialized = false;

		float red,
		  green,
		  blue,
		  alpha;
	}

	void Init (int width, int height, const std::string& title) {
		// Initialize SDL for the first display only
		SDL_Init (SDL_INIT_VIDEO);

		// Set the OpenGL versions to be used (in this case, 3.2)
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Set in bits:
		SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);   // The amount of red.
		SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);  // The amount of blue.
		SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8); // The amount of green.
		SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8); // The amount of alpha.

		// Enable double buffering to avoid a flashing
		// screen as it is cleared.
		SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

		// Create the window
		detail::window = SDL_CreateWindow (
		  title.c_str (), // Window title. NOTE: It must be a C-style string
		  // (char*) so it must be converted with a call to "c_str()".
		  SDL_WINDOWPOS_CENTERED,
		  SDL_WINDOWPOS_CENTERED, // Window position according to the screen
		  width,
		  height,             // Window width and height
		  SDL_WINDOW_OPENGL); // Flags for the window

		SDL_assert (detail::window != nullptr);

		detail::glContext = SDL_GL_CreateContext (detail::window);
		SDL_assert (gladLoadGL ());

		/*
		glewExperimental = true;
		auto i = glewInit();
		DEBUG_OUT(i);
		SDL_assert(i == GLEW_OK);
		glGetError();
		*/

		detail::initialized = true;

		detail::width  = width;
		detail::height = height;
		detail::title  = title;
	}

	void Clear (float red, float green, float blue, float alpha) {
		SetClearColor (red, green, blue, alpha);
		Clear ();
	}

	void SetClearColor (float red, float green, float blue, float alpha) {
		glClearColor (red, green, blue, alpha);

		detail::red   = red;
		detail::green = green;
		detail::blue  = blue;
		detail::alpha = alpha;
	}

	void Clear () {
		SDL_GL_SwapWindow (detail::window);
		glClear (GL_COLOR_BUFFER_BIT);
	}

	void Close () {
		// Destroy the window of the current display
		SDL_DestroyWindow (detail::window);

		SDL_GL_DeleteContext (detail::glContext);
		SDL_Quit ();
	}

	void Resize (int newWidth, int newHeight) {
		detail::width  = newWidth;
		detail::height = newHeight;

		SDL_SetWindowSize (detail::window, newWidth, newHeight);
	}
}
}
