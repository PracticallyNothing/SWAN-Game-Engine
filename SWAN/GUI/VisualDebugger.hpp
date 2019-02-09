#ifndef SWAN_VISUAL_DEBUGGER_HPP
#define SWAN_VISUAL_DEBUGGER_HPP

#include "Core/Defs.hpp"
#include "Core/Format.hpp"
#include "Core/Logging.hpp"
#include "GUIManager.hpp"
#include "IGUIElement.hpp"
#include <functional>

namespace SWAN
{
	class VisualDebugger : public IGUIElement
	{
	  public:
		VisualDebugger(const BitmapFont* font)
		    : Font(font) {}
		// bool OnKeyPress(Key key) override
		// {
		//     return false;
		// }

		~VisualDebugger() override
		{
		}

		double yOffset;
		bool OnMouseScroll(MouseScroll scroll) override
		{
			yOffset = std::min<double>(yOffset + scroll.Amount * 15, 0);
			return true;
		}

		Rect2D GetRect() const override
		{
			return { 0, 0, Display::GetWidth(), Display::GetHeight() };
		}

		void OnRender(GUIManager& man) override
		{
			Vector<Rect2D> Rectangles;
			// Background
			Rectangles.emplace_back(0, 0, Display::GetWidth(), Display::GetHeight(), vec4{ 0, 0, 0, 0.5 });

			// Log Background
			Rectangles.emplace_back(
			    10, 10 * 2 + Font->getGlyphHeight(),
			    Display::GetWidth() - 20, Display::GetHeight() - (10 * 4 + Font->getGlyphHeight()),
			    vec4{ 0, 0, 0, 0.5 });

			// Log Rectangles
			int x = 30;
			int y = 10 * 2 + Font->getGlyphHeight() + 10 + yOffset;

			for(int i = 0; i < Lines.size(); i++) {
				Rectangles.emplace_back(
				    x, y,
				    Display::GetWidth() - 2 * x, 10 + Font->getTextHeight(Lines[i]),
				    vec4{ 0, 0, 0, 0.5 });
				Rectangles.back().ClipAgainst(Rectangles[1]);

				y += 20 + Font->getTextHeight(Lines[i]);
			}

			// Final Render
			man.BatchRenderRect2D(Rectangles);

			// Window Title
			man.RenderText(
			    Display::GetWidth() / 2 - Font->getTextWidth("Log") / 2,
			    10,
			    Font, "Log");

			x = 30;
			y = 10 * 2 + Font->getGlyphHeight() + 10 + yOffset;

			for(int i = 0; i < Lines.size(); i++) {
				man.RenderTextClipped(x + 10, y + 5, Font, Lines[i], Rectangles[i + 2]);
				y += 20 + Font->getTextHeight(Lines[i]);
			}
		}

	  private:
		const BitmapFont* Font;
		Vector<String> Lines = {
			"I like beans!",
			"I like beans too!",
			"I don't like beans -\nI hate you.",
			"...",
			"I like beans!",
			"...",
			"I like beans!",
			"I like beans too!",
			"I don't like beans -\nI hate you.",
			"...",
			"I like beans!",
			"...",
			"I like beans!",
			"I like beans too!",
			"I don't like beans -\nI hate you.",
			"...",
			"I like beans!",
			"...",
			"I like beans!",
			"I like beans too!",
			"I don't like beans -\nI hate you.",
			"...",
			"I like beans!",
		};
	};
} // namespace SWAN

#endif
