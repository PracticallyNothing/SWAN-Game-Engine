#include "Display.hpp"
#include "Format.hpp"
#include "Logging.hpp"

#include "Utility/Debug.hpp"
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "OpenGL/OnGLInit.hpp"

namespace SWAN
{
	namespace Display
	{
		namespace detail
		{
			SDL_Window* window;
			SDL_GLContext glContext;

			std::string title;
			int width = -1, height = -1;
			bool initialized = false;

			float red, green, blue, alpha;
		} // namespace detail

		void Init(int width, int height, const std::string& title)
		{
			Log("Display", "Initializing...", LogLevel::Info);
			// Initialize SDL for the first display only
			if(detail::initialized) {
				Log("Display", "Display already initialized.", LogLevel::Warning);
				return;
			} else if(SDL_Init(SDL_INIT_VIDEO) < 0) {
				Log("Display", Format("SDL failed to initialize! SDL error: {}", SDL_GetError()), LogLevel::Fatal);
			} else {
				Log("Display", "SDL initialized.", LogLevel::Success);
			}

			// Log("Display|OpenGL", "Setting OpenGL version to 3.3 core...", LogLevel::Info);
			// Set the OpenGL versions to be used (in this case, 3.3)
			if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0)
				Log("Display|OpenGL", Format("Failed to set OpenGL major version to 3. SDL error: {}", SDL_GetError()), LogLevel::Fatal);
			if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) < 0)
				Log("Display|OpenGL", Format("Failed to set OpenGL minor version to 3. SDL error: {}", SDL_GetError()), LogLevel::Fatal);
			if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
				Log("Display|OpenGL", Format("Failed to set OpenGL profile to core. SDL error: {}", SDL_GetError()), LogLevel::Error);
			Log("Display|OpenGL", "Set OpenGL profile to 3.3 core.", LogLevel::Success);

			// Set in bits:
			if(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) < 0) // The amount of red.
				Log("Display|OpenGL", Format("Failed to set red bits to 8. SDL error: {}", SDL_GetError()), LogLevel::Error);
			if(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) < 0) // The amount of green.
				Log("Display|OpenGL", Format("Failed to set green bits to 8. SDL error: {}", SDL_GetError()), LogLevel::Error);
			if(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) < 0) // The amount of blue.
				Log("Display|OpenGL", Format("Failed to set blue bits to 8. SDL error: {}", SDL_GetError()), LogLevel::Error);
			if(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) < 0) // The amount of alpha.
				Log("Display|OpenGL", Format("Failed to set alpha bits to 8. SDL error: {}", SDL_GetError()), LogLevel::Error);
			Log("Display|OpenGL", "Set OpenGL BPP to 8 bits per red, green, blue and alpha.", LogLevel::Success);

			// Enable double buffering to avoid a flashing
			// screen as it is cleared.
			if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
				Log("Display|OpenGL", Format("Failed to enable double buffering. SDL error: {}", SDL_GetError()), LogLevel::Error);
			Log("Display|OpenGL", "Enabled double buffering.", LogLevel::Success);

			// Log("Display", "Initializing display window...", LogLevel::Info);
			// Create the window
			detail::window = SDL_CreateWindow(title.c_str(), // Window title.
			                                  SDL_WINDOWPOS_CENTERED,
			                                  SDL_WINDOWPOS_CENTERED, // Window position according to the screen
			                                  width,
			                                  height,
			                                  SDL_WINDOW_OPENGL); // Flags for the window
			if(!detail::window)
				Log("Display", Format("Failed to initialize display window. SDL error: {}", SDL_GetError()), LogLevel::Fatal);
			else
				Log("Display", "Initialized display window.", LogLevel::Success);

			detail::glContext = SDL_GL_CreateContext(detail::window);

			if(!detail::glContext)
				Log("Display|OpenGL", Format("Failed to initialize OpenGL context. SDL error: {}", SDL_GetError()), LogLevel::Fatal);
			else
				Log("Display", "Initialized OpenGL context.", LogLevel::Success);

			int ok = gladLoadGL();
			if(!ok)
				Log("Display|OpenGL", Format("GLAD failed to load OpenGL functions. Error code: {}", ok), LogLevel::Fatal);
			Log("Display|OpenGL", "OpenGL functions loaded.", LogLevel::Success);

			detail::initialized = true;

			detail::width = width;
			detail::height = height;
			detail::title = title;

			SWAN::detail::finishInit();
			Log("Display", "Initialized display.", LogLevel::Success);
		}

		void Clear(float red, float green, float blue, float alpha)
		{
			SetClearColor(red, green, blue, alpha);
			Clear();
		}

		void SetClearColor(float red, float green, float blue, float alpha)
		{
			Log("Display",
			    Format("Setting background color to (r: {}, g: {}, b: {}, a: {}).",
			           red, green, blue, alpha),
			    LogLevel::Debug);

			glClearColor(red, green, blue, alpha);

			detail::red = red;
			detail::green = green;
			detail::blue = blue;
			detail::alpha = alpha;
		}

		void Clear()
		{
			SDL_GL_SwapWindow(detail::window);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void Close()
		{
			// Destroy the window of the current display
			SDL_DestroyWindow(detail::window);

			SDL_GL_DeleteContext(detail::glContext);
			SDL_Quit();

			detail::initialized = false;

			Log("Display", "Closed display.", LogLevel::Info);
		}

		void Resize(int newWidth, int newHeight)
		{
			Log("Display",
			    Format("Resized display from {}x{} to {}x{}.",
			           detail::width, detail::height, newWidth, newHeight),
			    LogLevel::Debug);

			detail::width = newWidth;
			detail::height = newHeight;

			SDL_SetWindowSize(detail::window, newWidth, newHeight);
			glViewport(0, 0, detail::width, detail::height);
		}
	} // namespace Display
} // namespace SWAN
