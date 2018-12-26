#ifndef SWAN_GUI_TEXTBOX_HPP
#define SWAN_GUI_TEXTBOX_HPP

#include "Core/Defs.hpp"
#include "GUIManager.hpp"
#include "IGUIElement.hpp"
#include "Core/Logging.hpp"
#include "Core/Format.hpp"
#include "Core/Buffer.hpp"

namespace SWAN
{
    struct TextBox : public IGUIElement
    {
	TextBox(Rect2D rect, const BitmapFont* font, String text)
	    : Rectangle(rect), Font(font), Buff()
	{
	    Buff.ReplaceCurrentLine(text);
	}

	bool OnGainFocus() override { Active = true; return true; }
	bool OnLoseFocus() override { Active = false; return true; }

	bool OnMouseKeyRelease(MouseKey key) override
	{
	    return true;
	}

	bool OnMouseScroll(MouseScroll scroll) override
	{
	    TextOffset.y += scroll * 10;
	    return true;
	}

	bool OnKeyPress(Key key) override
	{
	    switch(key.Code) {
	    case LeftArrow: Buff.Left(); break;
	    case RightArrow: Buff.Right(); break;
	    case UpArrow: Buff.Up(); break;
	    case DownArrow: Buff.Down(); break;

	    case Backspace: Buff.DeleteAtCursor(); break;

	    case Insert: InsertMode = !InsertMode; break;
	    case Delete: Buff.DeleteAfterCursor(); break;
	    case Home: Buff.SeekBOL(); break;
	    case End: Buff.SeekEOL(); break;
	    case PageUp: Buff.Up(10); break;
	    case PageDown: Buff.Down(10); break;
	    default: {
		char c = KeyAsChar(key);
		if(c != '\0') {
		    if(InsertMode) {
			Buff.OverwriteAtCursor(c);
		    } else {
			Buff.InsertAtCursor(c);
		    }
		}
	    }
	    }

	    return true;
	}

	bool OnKeyRepeat(Key key) override {return OnKeyPress(key);}
	bool OnKeyHold(Key key) override { return true; }
	bool OnKeyRelease(Key key) override { return true; }

	Rect2D GetRect() const override { return Rectangle; }

	void OnRender(GUIManager& renderer) override
	{
	    renderer.RenderRect2D(Rectangle);
	    renderer.RenderTextClipped(
		(Rectangle.Position + TextOffset).x, (Rectangle.Position + TextOffset).y,
		Font, Buff,
		Rectangle
	    );

	    Rect2D cursor = Rect2D(
		(Rectangle.Position + TextOffset).x + Buff.GetCursorX() * Font->getGlyphWidth(),
		(Rectangle.Position + TextOffset).y + Buff.GetCursorY() * Font->getGlyphHeight(),
		2,
		Font->getGlyphHeight()
	    );
	    if(Active)
		renderer.RenderRect2DClipped(cursor, Rectangle);
	}

	bool Active = false;
	bool Editable = false;
	bool InsertMode = false;
	ivec2 TextOffset = {0, 0};
	Rect2D Rectangle;
	const BitmapFont* Font;
	Buffer Buff;
    };
}

#endif
