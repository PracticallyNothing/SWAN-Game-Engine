#include "MouseEvent.hpp"
#include "Core/Display.hpp"
#include "FlatInputRep.hpp"

#include "Core/Logging.hpp"

namespace SWAN
{
	std::deque<MouseMove> MouseWarps;

	void WarpMouseTo(int x, int y)
	{
		MouseMove res(x, y);
		res.RelativeX = GetFlatRep().MouseState.X - x;
		res.RelativeY = GetFlatRep().MouseState.Y - y;
		MouseWarps.push_back(res);

		SDL_WarpMouseInWindow(Display::detail::window, x, y);
		GetFlatRep().MouseState.X = x;
		GetFlatRep().MouseState.Y = y;
	}
} // namespace SWAN
