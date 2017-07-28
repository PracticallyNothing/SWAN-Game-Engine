#include "Display.hpp"

#include <SDL2/SDL.h>

#include <GL/glew.h>

uint Display::numDisplays = 0;
SDL_GLContext Display::glContext = 0;

void Display::clear() {
    SDL_GL_SwapWindow(win);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::setClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Display::focus() {
    glViewport(0, 0, w, h);
    SDL_GL_MakeCurrent(win, Display::glContext);
}

Display::Display(int w, int h, const std::string& title) : w(w), h(h) {
    // If this is the first display initialize all attributes
    // needed for the proper functioning
    // of OpenGL inside the SDL_GLContext.
    if (!Display::numDisplays) {
        // Initialize SDL for the first display only
        SDL_Init(SDL_INIT_VIDEO);

		// Set the OpenGL versions to be used (in this case, 3.2)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

        // Set in bits:
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);    // The amount of red.
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);   // The amount of blue.
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);  // The amount of green.
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);  // The amount of alpha.

        // Enable double buffering to avoid a flashing
        // screen as it is cleared.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    }

    // Create the window
    win = SDL_CreateWindow(
        title.c_str(),  // Window title. NOTE: It must be a C-style string
                        // (char*) so it must be converted with a call to
                        // "c_str()".
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,  // Window position according to the screen
        w, h,                    // Window width and height
        SDL_WINDOW_OPENGL);      // Flags for the window

    if (!Display::numDisplays) {
        Display::glContext = SDL_GL_CreateContext(win);
        glewExperimental = true;
		glewInit();
		glGetError();
    }

    Display::numDisplays++;
}

Display::~Display() {
    // Destroy the window of the current display
    SDL_DestroyWindow(win);
    // Lower the number of displays left
    Display::numDisplays--;

    // If this was the last display (if numDisplays is 0)
    // delete the SDL_GLContext we were using
    // because we don't need it any more.
    if (!Display::numDisplays) {
        SDL_GL_DeleteContext(Display::glContext);
        SDL_Quit();
    }
}
