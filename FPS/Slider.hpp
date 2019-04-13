#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <functional>

#include "SWAN/Core/Format.hpp"
#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/GUI/IGUIElement.hpp"
#include "SWAN/Rendering/BitmapFont.hpp"

struct Slider : public SWAN::IGUIElement {
	Slider(SWAN::Rect2D clip) : Clip(clip) {}
	std::function<void(float)> Callback = [](float f) {};
	float Value;

	SWAN::String label = "";
	bool showLabel = false,
	     showValue = false;

	const SWAN::BitmapFont* font = nullptr;

	bool OnMouseEnter(SWAN::MouseMove move) override { return true; }
	bool OnMouseLeave(SWAN::MouseMove move) override
	{
		_showPseudoRect = false;
		return true;
	}

	bool OnMouseMoveInside(SWAN::MouseMove move)
	{
		_showPseudoRect = BGRect.IsPointInside({ move.X, move.Y });

		if(_showPseudoRect) {
			float pseudoValue = (move.X - BGRect.Position.x) / BGRect.Size.x;
			PseudoRect.Size.x = BGRect.Size.x * pseudoValue;
		}

		return true;
	}
	bool OnMouseKeyPress(SWAN::MouseKey key)
	{
		return true;
	}
	bool OnMouseKeyRelease(SWAN::MouseKey key)
	{
		if(_showPseudoRect) {
			ActiveRect.Size.x = PseudoRect.Size.x;
			Value = ActiveRect.Size.x / BGRect.Size.x;
			Callback(Value);
		}

		return true;
	}

	bool OnMouseScroll(SWAN::MouseScroll ms)
	{
		return true;
	}

	SWAN::Rect2D GetRect() const override
	{
        return Clip;
	}

	void OnRender(SWAN::GUIManager& renderer) override
	{
		renderer.BatchRenderRect2D({ BGRect, ActiveRect });
		if(_showPseudoRect)
			renderer.RenderRect2D(PseudoRect);
		if(showLabel && font && !label.empty())
			renderer.RenderText(Clip.Position.x, Clip.Position.y, font, label);
		if(showValue && font)
			renderer.RenderText(BGRect.Position.x, BGRect.Position.y, font, SWAN::Format("{}", Value));
	}

	SWAN::Rect2D BGRect, ActiveRect, PseudoRect;
	bool _showPseudoRect = false;

	SWAN::Rect2D Clip;
};

#endif