#ifndef SWAN_WINDOW_EVENT_HPP
#define SWAN_WINDOW_EVENT_HPP

namespace SWAN
{
	/// A structure representing the window getting resized.
	struct WindowResize {
		/// Default constructor.
		WindowResize() {}
		/** @brief Construct a WindowResize event 
	 *          with the given parameters.
	 *
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
		WindowResize(unsigned width, unsigned height)
		    : Width(width), Height(height) {}

		unsigned Width;  ///< The new width of the window.
		unsigned Height; ///< The new height of the display.
	};
} // namespace SWAN

#endif
