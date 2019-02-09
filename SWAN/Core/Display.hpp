#ifndef SWAN_DISPLAY_HPP
#define SWAN_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <string>

#include "Maths/Vector.hpp"

typedef unsigned int uint;

namespace SWAN
{
	namespace Display
	{
		namespace detail
		{
			/// (Internal) Handle for SDL window.
			extern SDL_Window* window;

			/// (Internal) Handle for SDL OpenGL context.
			extern SDL_GLContext glContext;

			/// (Internal) Display's title.
			/// NOTE: Setting this manually does nothing.
			extern std::string title;

			/// (Internal) Width and height of the display in pixels.
			/// NOTE: Setting these manually does nothing, use Init() or Resize().
			extern int width, height;

			/// (Internal) Whether a display has actually been initialized
			/// NOTE: Setting this manually might introduce errors, use Init() or Close().
			extern bool initialized;

			/// (Internal) Clear color values.
			/// NOTE: Setting these manually does nothing, use SetClearColor().
			extern float red, green, blue, alpha;
		} // namespace detail

		/**
		 * @brief Initializes a Display onto which to render.
		 * 
		 * @param width The width of the display in pixels
		 * @param height The height of the display in pixels
		 * @param title The title of the display 
		 */
		extern void Init(int width, int height, const std::string& title);

		/** @brief Clear the display. */
		extern void Clear();

		/**
		 * @brief Clear the display with the given clearing color.
		 *
		 * NOTE: The clear color will change to the one specified in the arguments.
		 *
		 * @param red Value for red. Clamped between 0.0 and 1.0.
		 * @param green Value for green. Clamped between 0.0 and 1.0.
		 * @param blue Value for blue. Clamped between 0.0 and 1.0.
		 * @param alpha Value for alpha. Clamped between 0.0 and 1.0.
		 */
		extern void Clear(float red, float green, float blue, float alpha);

		/** @brief Close the display and clean up remaining stuff. */
		extern void Close();

		/**
		 * @brief Give the display a new size.
		 *
		 * @param newWidth The new width for the display.
		 * @param newHeight The new height for the display.
		 */
		extern void Resize(int newWidth, int newHeight);

		/**
		 * @brief Set the color with which the display will be cleared.
		 *
		 * @param red Value for red. Clamped between 0.0 and 1.0.
		 * @param green Value for green. Clamped between 0.0 and 1.0.
		 * @param blue Value for blue. Clamped between 0.0 and 1.0.
		 * @param alpha Value for alpha. Clamped between 0.0 and 1.0.
		 */
		extern void SetClearColor(float red, float green, float blue, float alpha);

		/**
		 * @brief Get the clearing color.
		 *
		 * NOTE: If any of detail::red, detail::green, detail::blue or detail::alpha
		 *       have been changed manually after a call to SetClearColor(),
		 *       the result will be incorrect.
		 *
		 * @return A SWAN::vec4 containing the color.
		 */
		inline vec4 GetClearColor()
		{
			return vec4(detail::red, detail::green, detail::blue, detail::alpha);
		}

		/** @brief Has the display already been initialized? */
		inline bool IsInitialized() { return detail::initialized; }

		/** 
		 * @brief Get the aspect ratio of the display.
		 *
		 * NOTE: If detail::width or detail::height have been changed manually 
		 *       after a call to Init() or Resize(), the result will be incorrect.
		 *
		 * @return The aspect ratio.
		 */
		inline double GetAspectRatio()
		{
			return (double) detail::width / detail::height;
		}

		/** 
		 * @brief Get the width of the display.
		 *
		 * NOTE: If detail::width has been changed manually 
		 *       after a call to Init() or Resize(), the result will be incorrect.
		 *
		 * @return The width in pixels.
		 */
		inline int GetWidth() { return detail::width; }

		/** 
		 * @brief Get the height of the display.
		 *
		 * NOTE: If detail::height has been changed manually 
		 *       after a call to Init() or Resize(), the result will be incorrect.
		 *
		 * @return The height in pixels.
		 */
		inline int GetHeight() { return detail::height; }
	} // namespace Display
} // namespace SWAN

#endif // !DISPLAY_HPP
