#ifndef SWAN_INPUT_FRAME_HPP
#define SWAN_INPUT_FRAME_HPP

#include <string>

#include "../Core/Display.hpp"

#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "WindowEvent.hpp"

namespace SWAN
{
	/// @addtogroup Input
	/// @{

	/** @brief An interface for an object 
     *         (the GUI, a text box, the game window itself) that receives input.
     *
     * The input frame is an abstract class that receives events and acts upon those events.
     * At one moment, there can be only one input frame active, which can be controlled via
     * SWAN::SetCurrentInputFrame(InputFrame* frame).
     * The current input frame will receive all events from the SWAN event queue after a call to
     * SWAN::UpdateEvents().
     * 
     * @note A concrete input frame may implement as many of the functions as it thinks it needs,
     *       however, any unprocessed input may disappear, unless the implementation itself passes
     *       it on to another input frame.
     */
	class InputFrame
	{
	  public:
		/// Virtual destructor.
		virtual ~InputFrame() {}

		/** @brief Callback for whenever a key on the keyboard is pressed.
	 *
	 * @note This only activates at the start of a key press.\n
	 *       If you want to do something whenever the key is being **held down**,
	 *       (e.g. holding the **A** key inside of an **FPS**) use OnKeyHold(Key key).\n
	 *       If you want to do something every time the key's been **repeatedly pressed** 
	 *       (e.g. holding the key **A** inside of a **text editor**) use OnKeyRepeat(Key key).
	 *
	 * @param key The key that's been pressed.
	 */
		virtual void OnKeyPress(Key key) {}

		/** @brief Callback for whenever a key on the keyboard 
	 *         is reported as pressed while being held down.
	 *
	 * A key is considered to repeat whenever the OS 
	 * reports that the key has been pressed again 
	 * (e.g. inside a text editor, holding a letter key will insert it once, 
	 *  then, after a delay, start inserting it again).
	 * 
	 * @param key The key that's repeating.
	 */
		virtual void OnKeyRepeat(Key key) {}
		/** @brief Callback for whenever the key has been released.
	 * 
	 * A key is released whenever you lift your finger off it.
	 *
	 * @param key The key that's been released.
	 */
		virtual void OnKeyRelease(Key key) {}
		/** @brief Callback for whenever a key on the keyboard is being held down.
	 *
	 * A key is considered to be held down whenever 
	 * it's been pressed and it still hasn't been released.
	 * (e.g. inside an FPS, holding W will move you forward until you let go).
	 * 
	 * @param key The key that's being held down.
	 */
		virtual void OnKeyHold(Key key) {}

		/** @brief Callback for whenever a mouse button is pressed.
	 *
	 * @param button The mouse button which was pressed.
	 */
		virtual void OnMouseKeyPress(MouseKey key) {}
		/** @brief Callback for whenever a mouse button is released.
	 *
	 * @param button The mouse button which was released. 
	 */
		virtual void OnMouseKeyRelease(MouseKey key) {}
		/** @brief Callback for whenever the mouse is moved. 
	 *
	 * @param move MouseMove with information on how the mouse has moved. 
	 */
		virtual void OnMouseMove(MouseMove move) {}
		/** @brief Callback for whenever the scroll wheel on the mouse is used. 
	 *
	 * @param scroll The amount the mouse has scrolled. 
	 */
		virtual void OnMouseScroll(MouseScroll scroll) {}

		/** @brief Callback for whenever the window changes size.
	 *
	 * By default, just resizes the display by calling Display::Resize().
	 *
	 * @param resize WindowResize with information about the new window's size.
	 */
		virtual void OnWindowResize(WindowResize resize) { Display::Resize(resize.Width, resize.Height); }

		/** @brief Callback for whenever the window loses keyboard focus. */
		virtual void OnWindowLoseKeyboardFocus() {}

		/** @brief Callback for whenever the window regains keyboard focus. */
		virtual void OnWindowRegainKeyboardFocus() {}

		/** @brief Callback for whenever the window loses mouse focus. */
		virtual void OnWindowMouseLeave() {}
		/** @brief Callback for whenever the window regains mouse focus. */
		virtual void OnWindowMouseEnter() {}

		/** @brief Callback for whenever the window is being closed. 
	 *
	 * By default calls Display::Close().
	 *
	 * @note This will not be triggered, unless the window manager itself
	 *       requests that the window be closed (the user has clicked the "X" button).
	 */
		virtual void OnWindowExit() { Display::Close(); }
	};

	namespace detail
	{
		/// A pointer to the current input frame.
		extern InputFrame* currentInputFrame;
	} // namespace detail

	/// Get the current active input frame.
	inline InputFrame* GetCurrentInputFrame() { return detail::currentInputFrame; }

	/** @brief Set the current active input frame.
     *
     * The current input frame may be set to any pointer to an implementation of the interface.
     * It may also be set to nullptr if no input is to be processed.
     *
     * @note It's probably a very bad idea to set the current input frame to nullptr,
     *       since you'll have no way of controlling anything, not even closing the window.
     *
     * @param frame A pointer to the frame that will become the active frame.
     */
	inline void SetCurrentInputFrame(InputFrame* frame) { detail::currentInputFrame = frame; }

	/// @}
} // namespace SWAN

#endif
