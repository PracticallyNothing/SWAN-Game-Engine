#ifndef SWAN_FLAT_INPUT_REP_HPP
#define SWAN_FLAT_INPUT_REP_HPP

#include "KeyEvent.hpp"

namespace SWAN
{
	/** @brief The final state of all inputs after the last SWAN::UpdateEvents() call.
     *
     * Records the state itself, but does not record the time at which it has changed.
     */
	struct FlatInputRep {
		/** @brief Array of key states.
	 *
	 * True means a key is being held down, false means it's been released. 
	 */
		bool Keys[KeyCode::TotalKeys] = { false };

		/// A record of the mouse's state.
		struct Mouse {
			int X, Y;

			int ScrollAmount = 0;

			bool LMB = false;
			bool MMB = false;
			bool RMB = false;

			inline bool IsOverRect(int x, int y, int w, int h) const
			{
				return X >= x && X <= x + w && Y >= y && Y <= y + h;
			}
		} MouseState;
	};

	namespace detail
	{
		extern FlatInputRep flatInput, prevFlatInput;
	}
	inline FlatInputRep& GetFlatRep() { return detail::flatInput; }
	inline FlatInputRep& GetPrevFlatRep() { return detail::prevFlatInput; }
} // namespace SWAN

#endif
