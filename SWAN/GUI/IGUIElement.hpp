#ifndef SWAN_IGUIELEMENT_HPP
#define SWAN_IGUIELEMENT_HPP

#include "Input/KeyEvent.hpp"
#include "Input/MouseEvent.hpp"
#include "Rect2D.hpp"

namespace SWAN
{
	struct GUIManager;

	struct IGUIElement {
		/// Virtual destructor.
		virtual ~IGUIElement() {}

		// ----- Input handling ----- //

		/** @brief Callback for whenever a key is pressed.
	 *
	 * @note Keyboard events are only received 
	 *       when the element has keyboard focus.
	 */
		virtual bool OnKeyPress(Key key) { return false; }
		/** @brief Callback for whenever a key press is repeated by the OS.
	 *
	 * @note Keyboard events are only received 
	 *       when the element has keyboard focus.
	 */
		virtual bool OnKeyRepeat(Key key) { return false; }
		/** @brief Callback for whenever a key is released.
	 *
	 * @note Keyboard events are only received 
	 *       when the element has keyboard focus.
	 */
		virtual bool OnKeyRelease(Key key) { return false; }
		/** @brief Callback for whenever a key is held down.
	 *
	 * @note Keyboard events are only received 
	 *       when the element has keyboard focus.
	 */
		virtual bool OnKeyHold(Key key) { return false; }

		/** @brief Callback for whenever the mouse enters the element.
	 *
	 * Whether the mouse is inside or outside a given element is decided
	 * by its rendering rectangle (See GetRenderRect()).
	 */
		virtual bool OnMouseEnter(MouseMove move) { return false; }
		/** @brief Callback for whenever the mouse leaves the element.
	 *
	 * Whether the mouse is inside or outside a given element is decided
	 * by its rendering rectangle (See GetRenderRect()).
	 */
		virtual bool OnMouseLeave(MouseMove move) { return false; }

		/** @brief Callback for whenever the element gains keyboard focus. */
		virtual bool OnGainFocus() { return false; }
		/** @brief Callback for whenever the element loses keyboard focus. */
		virtual bool OnLoseFocus() { return false; }

		/** @brief Callback for whenever a mouse button is pressed while hovering over the element. */
		virtual bool OnMouseMoveInside(MouseMove move) { return false; }
		/** @brief Callback for whenever a mouse button is pressed while hovering over the element. */
		virtual bool OnMouseKeyPress(MouseKey key) { return false; }
		/** @brief Callback for whenever a mouse button is released while hovering over the element. */
		virtual bool OnMouseKeyRelease(MouseKey key) { return false; }
		/** @brief Callback for whenever the scrollwheel is used over the element. */
		virtual bool OnMouseScroll(MouseScroll ms) { return false; }

		// ----- Rendering ----- //

		/** @brief Returns the rectangle inside which the element resides.
	 *
	 * The rectangle acts as the element's bounding box, which tells the GUIRenderer whether the mouse is
	 * inside or outside the given element.
	 */
		virtual Rect2D GetRect() const = 0;

		virtual void OnRender(GUIManager& renderer) = 0;
	};
} // namespace SWAN

#endif
