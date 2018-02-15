#ifndef SWAN_DISPLAY_HPP
#define SWAN_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>

typedef unsigned int uint;

namespace SWAN {
namespace Display {
	namespace detail {
		extern SDL_Window* window;
		extern SDL_GLContext glContext;

		extern std::string title;
		extern int width, height;
		extern bool initialized;

		extern float red, green, blue, alpha;
	} // namespace detail

	extern void Init(int width, int height, const std::string& title);
	extern void Clear();
	extern void Clear(float red, float green, float blue, float alpha);
	extern void Close();
	extern void Resize(int newWidth, int newHeight);

	extern void SetClearColor(float red, float green, float blue, float alpha);
	inline glm::vec4 GetClearColor() {
		return glm::vec4(detail::red, detail::green, detail::blue, detail::alpha);
	}

	inline bool IsInitialized() { return detail::initialized; }

	inline double GetAspectRatio() {
		return (double) detail::width / detail::height;
	}

	inline int GetWidth() { return detail::width; }
	inline int GetHeight() { return detail::height; }
} // namespace Display
} // namespace SWAN

#endif // !DISPLAY_HPP
