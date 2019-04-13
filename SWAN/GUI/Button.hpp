#ifndef SWAN_GUI_BUTTON_HPP
#define SWAN_GUI_BUTTON_HPP

#include "GUIManager.hpp"
#include "IGUIElement.hpp"
#include <functional>

namespace SWAN
{
	class Button : public IGUIElement
	{
		Button(int x, int y, size_t w, size_t h)
		{
			rect.Position = SWAN::ivec2(x, y);
			rect.Size = SWAN::ivec2(w, h);
			rect.Fix();
		}

		bool OnMouseEnter(MouseMove move) override
		{
			return true;
		}

		bool OnMouseLeave(MouseMove move) override
		{
			return true;
		}

		Rect2D rect;
	};
} // namespace SWAN

#endif
