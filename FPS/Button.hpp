#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

#include <functional>

#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/GUI/IGUIElement.hpp"
#include "SWAN/Rendering/BitmapFont.hpp"

class Button : public SWAN::IGUIElement
{
  public:
	Button(int x, int y, int width, int height)
	{
		Rect = SWAN::Rect2D(x, y, width, height);
	}

	void SetPosition(int x, int y)
	{
		Rect.Position.x = x;
		Rect.Position.y = y;
	}

	void SetSize(int x, int y)
	{
		Rect.Size.x = x;
		Rect.Size.y = y;
		Rect.Fix();
	}

	void SetText(const SWAN::BitmapFont* font, SWAN::String txt)
	{
		Font = font;
		Text = txt;
	}

	bool OnMouseEnter(SWAN::MouseMove move) override
	{
		Active = true;
		return true;
	}
	bool OnMouseLeave(SWAN::MouseMove move) override
	{
		Active = false;
		return true;
	}
	bool OnMouseKeyRelease(SWAN::MouseKey key) override
	{
		if(Active) {
			Action();
			return true;
		}
		return false;
	}

	void OnRender(SWAN::GUIManager& man) override
	{
		SWAN::vec4 ActiveColor = { 0.5, 0.5, 0.7, 1 },
		           InactiveColor = { 0.3, 0.3, 0.5, 1 };

		if(Active)
			Rect.SetColor(ActiveColor);
		else
			Rect.SetColor(InactiveColor);

		man.RenderRect2D(Rect);

		if(Font && Text.length()) {
			SWAN::ivec2 TextPos = Rect.Position + Rect.Size / 2;
			TextPos.x -= Font->getTextWidth(Text) / 2;
			TextPos.y -= Font->getTextHeight(Text) / 2;
			man.RenderText(TextPos.x, TextPos.y, Font, Text);
		}
	}

	SWAN::Rect2D GetRect() const override { return Rect; }

	void SetAction(std::function<void(void)> act)
	{
		Action = act;
	}

  private:
	bool Active = false;

	std::function<void(void)> Action = [] {};

	SWAN::Rect2D Rect;
	const SWAN::BitmapFont* Font = nullptr;
	SWAN::String Text;
};

#endif
