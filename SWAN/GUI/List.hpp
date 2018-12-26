#ifndef SWAN_GUI_LIST_HPP
#define SWAN_GUI_LIST_HPP

#include "GUIManager.hpp"
#include "IGUIElement.hpp"
#include "Core/Format.hpp"
#include "Core/Logging.hpp"
#include <functional>

namespace SWAN
{
    struct List : public IGUIElement {
	List(const BitmapFont* font, int x, int y)
	    : Pos(x, y), Font(font) {}

	virtual bool OnGainFocus() override { return true; }
	virtual bool OnLoseFocus() override { ActiveRect = -1; return true; }

	virtual bool OnMouseEnter(MouseMove move) override
	{
	    lmb = false;
	    MouseIsInside = true;
	    MousePos = {move.X, move.Y};
	    return true;
	}

	virtual bool OnMouseLeave(MouseMove move) override
	{
	    MouseIsInside = false;
	    lmb = false;
	    ActiveRect = -1;
	    return true;
	}

	virtual bool OnMouseMoveInside(MouseMove move) override
	{
	    MousePos = {move.X, move.Y};
	    if(lmb)
		ActiveRect = CalcActiveRect(move.X, move.Y);
	    return true;
	}

	virtual bool OnMouseScroll(MouseScroll scroll) override { return true; }

	virtual bool OnMouseKeyPress(MouseKey key) override
	{
	    if(key.Button == LMB) {
		lmb = true;
		ActiveRect = CalcActiveRect(key.X, key.Y);
	    }
	    return true;
	}

	virtual bool OnMouseKeyRelease(MouseKey key) override
	{
	    if(key.Button == LMB)
	    {
		Callback(Lines[CalcActiveRect(key.X, key.Y)]);
		ActiveRect = -1;
		lmb = false;
	    }
	    return true;
	}

	int CalcActiveRect(int MouseX, int MouseY)
	{
	    ivec2 mpos(MouseX, MouseY);
	    mpos -= GetRect().Position;
	    return mpos.y / (Font->getGlyphHeight() + Margin.y);
	}

	virtual bool OnKeyPress(Key key) override {
	    if(key.Code == DownArrow)
		ActiveRect = Util::Clamp<int>(ActiveRect + 1, 0, Lines.size() - 1);
	    else if(key.Code == UpArrow)
		ActiveRect = Util::Clamp<int>(ActiveRect - 1, 0, Lines.size() - 1);

	    if(key.Code == Enter && ActiveRect >= 0 && ActiveRect <= Lines.size())
		Callback(Lines[ActiveRect]);
		
	    return true;
	}

	virtual bool OnKeyRepeat(Key key) override { return OnKeyPress(key); }
	virtual bool OnKeyHold(Key key) override { return true; }
	virtual bool OnKeyRelease(Key key) override { return true; }

	virtual Rect2D GetRect() const override
	{
	    int width = Margin.x + MaxChars() * Font->getGlyphWidth();
	    int height = Lines.size() * (Margin.y + Font->getGlyphHeight());

	    return Rect2D(Pos, {width, height});
	}

	int MaxChars() const {
	    int maxLineLength = 0;

	    for(const String& s : Lines)
	    {
		if(s.length() > maxLineLength)
		    maxLineLength = s.length();
	    }

	    return maxLineLength;
	}

	Rect2D GenerateRect(int line) const
	{
	    if(line < 0 || line > Lines.size())
		return Rect2D(Pos + ivec2(0, line * Font->getGlyphHeight()), {0,0});

	    int width = MaxChars() * Font->getGlyphWidth();

	    Rect2D res(Pos + ivec2(0, (Margin.y + Font->getGlyphHeight()) * line),
		       ivec2(width, Font->getGlyphHeight()) + Margin);

	    return res;
	}

	virtual void OnRender(GUIManager& manager) override
	{
	    Vector<Rect2D> batch;
	    batch.reserve(Lines.size());

	    int width = Margin.x + MaxChars() * Font->getGlyphWidth();

	    for(int i = 0; i < Lines.size(); i++)
	    {
		bool odd = i % 2;
		vec4 color = (odd ? vec4(0.15, 0.15, 0.15, 1) : vec4(0.25, 0.25, 0.25, 1));
		Rect2D res = GenerateRect(i);

		if(i == ActiveRect)
		    color += vec4(0, 0, 0.5, 0);

		if(MouseIsInside && res.IsPointInside(MousePos))
		    color += vec4(0.05, 0.05, 0.05, 0);

		res.SetColor(color);

 		batch.push_back(res);
	    }
	    manager.BatchRenderRect2D(batch);

	    for(int i = 0; i < Lines.size(); i++)
	    {
		const Rect2D& r = batch[i];
		manager.RenderTextClipped(
		    r.Position.x + Margin.x / 2,
		    r.Position.y + Margin.y / 2,
		    Font, Lines[i], batch[i]
		);
	    }
	}

	const Vector<String>& GetLines() const { return Lines; }

	void AddLine(const String& line) { Lines.push_back(line); }
	void RemoveLine(int index) { Lines.erase(Lines.begin() + index); }
	void ClearLines() { Lines.clear(); }
    protected:
	bool lmb = false;
	std::function<void(const String&)> Callback = [this](const String& str){ this->Lines.front() = str; };

	bool MouseIsInside = false;
	ivec2 MousePos;

	int ActiveRect = -1;

	ivec2 Pos;
	ivec2 Margin = {6, 20};
	const BitmapFont* Font = nullptr;
	Vector<String> Lines;
    };
}

#endif
