#ifndef SWAN_EVENT_HPP
#define SWAN_EVENT_HPP

#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "WindowEvent.hpp"

namespace SWAN
{
	enum class EventType {
		None, ///< No event has happened.

		KeyPress,   ///< An event where a key was pressed, uses Event::KeyData.
		KeyHold,    ///< An event where a key is being held down, uses Event::KeyData.
		KeyRepeat,  ///< An event where a key is being repeated, uses Event::KeyData.
		KeyRelease, ///< An event where a key has been released, uses Event::KeyData.

		MouseKeyPress,   ///< An event where a mouse button was pressed, uses Event::MouseButtonData.
		MouseKeyRelease, ///< An event where a mouse button has been released, uses Event::MouseButtonData.
		MouseMove,       ///< An event where the mouse was moved, uses Event::MouseMoveData.
		MouseScroll,     ///< An event where the mouse scrolled, uses Event::MouseScrollData.

		WindowExit,        ///< An event where a window exit was triggered, doesn't use any members of Event.
		WindowResize,      ///< An event where the window was resized, uses Event::WindowResizeData.
		WindowLoseFocus,   ///< An event where the window lost keyboard focus, doesn't use any members of Event.
		WindowRegainFocus, ///< An event where the window regained keyboard focus, doesn't use any members of Event.
		WindowMouseLeave,  ///< An event where the window lost mouse focus, doesn't use any members of Event.
		WindowMouseEnter,  ///< An event where the window regained mouse focus, doesn't use any members of Event.
	};

	/// A structure that records an input event.
	struct Event {
		/// Default constructor.
		Event() : Type(EventType::None) {}
		/// Constructs an event of the given type.
		/// @param type The type to use.
		Event(EventType type) : Type(type) {}

		/// The type of the event, see EventType.
		EventType Type;

		/// Key data for when the event has occurred on the keyboard.
		Key KeyData;

		/// Data for when the event is a mouse press or release.
		MouseKey MouseButtonData;
		/// Data for when the event is the mouse moving.
		MouseMove MouseMoveData;
		/// Data for when the event is a mouse scroll.
		MouseScroll MouseScrollData;

		/// Data for when the event is the window being resized.
		WindowResize WindowResizeData;
	};

	extern void UpdateInputEvents();
} // namespace SWAN

#endif
