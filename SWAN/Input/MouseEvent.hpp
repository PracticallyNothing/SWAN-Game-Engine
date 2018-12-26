#ifndef SWAN_MOUSE_EVENT_HPP
#define SWAN_MOUSE_EVENT_HPP

#include <queue>

namespace SWAN
{
    /// @addtogroup Input
    /// @{

    /// An enum representing a mouse button.
    enum MouseButton {
	LeftMouseButton,   ///< The left mouse button.
	MiddleMouseButton, ///< The middle mouse button (might be dedicated key, might be scroll wheel).
	RightMouseButton,  ///< The right mouse button.

	LMB = LeftMouseButton,   ///< Abbreviation for LeftMouseButton.
	MMB = MiddleMouseButton, ///< Abbreviation for MiddleMouseButton.
	RMB = RightMouseButton   ///< Abbreviation for RightMouseButton.
    };

    /** @brief A mouse key press. */
    struct MouseKey {
	MouseButton Button; ///< The mouse button that was pressed or released.

	int X; ///< The mouse X position.
	int Y; ///< The mouse Y position.
    };

    /** @brief A mouse cursor location change. */
    struct MouseMove {
	/// Default constructor.
	MouseMove() {}

	constexpr bool operator==(const MouseMove& other) const {
	    return X == other.X
		&& Y == other.Y
		// && RelativeX == other.RelativeX
		// && RelativeY == other.RelativeY
		;
	}

	/** @brief Construct a MouseMove with the given parameters
	 *
	 * @param X The new X position of the mouse after the move.
	 * @param Y The new Y position of the mouse after the move.
	 * @param RelX The change in X position between this and the last move.
	 * @param RelY The change in Y position between this and the last move.
	 */
	MouseMove(int X, int Y, int RelX = 0, int RelY = 0)
	    : X(X), Y(Y), RelativeX(RelX), RelativeY(RelY) {}

	int X; ///< The new mouse X position.
	int Y; ///< The new mouse Y position.

	int RelativeX; ///< How much the mouse has moved along X relative to the last move.
	int RelativeY; ///< How much the mouse has moved along Y relative to the last move.
    };

    /** @brief A mouse scroll event. */
    struct MouseScroll {
	/** @brief The amount the mouse has scrolled.
	 *
	 * **Positive** value - the mouse scrolled **up**.\n
	 * **Negative** value - the mouse scrolled **down**.
	 */
	int Amount = 0;

	int X; ///< The mouse X position.
	int Y; ///< The mouse Y position.

	/** @brief Construct a mouse scroll event with a given amount of scrolling.
	 * @param amt The amount the mouse has scrolled. See Amount.
	 */
	MouseScroll(int amt = 0, int X = 0, int Y = 0)
	    : Amount(amt), X(X), Y(Y) {}

	/** @brief Convert a mouse scroll into an integer.
	 * @return The amount the mouse has scrolled. See Amount.
	 */
	constexpr operator int() const { return Amount; }
    };

    /// Forces the mouse cursor to move to a given location.
    extern void WarpMouseTo(int x, int y);

    /// @}
    extern std::deque<MouseMove> MouseWarps;
}

#endif
