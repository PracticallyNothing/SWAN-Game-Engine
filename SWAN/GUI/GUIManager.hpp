#ifndef SWAN_GUIMANAGER_HPP
#define SWAN_GUIMANAGER_HPP

#include "IGUIElement.hpp"

#include "Core/Defs.hpp"
#include "OpenGL/VAO.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/BitmapFont.hpp"
#include "Rendering/Text.hpp"

#include "Input/InputFrame.hpp"

#include "Rect2D.hpp"

namespace SWAN
{
    struct GUIManager: public InputFrame
    {
    public:
	void OnKeyPress(Key key) override;
	void OnKeyRepeat(Key key) override;
	void OnKeyRelease(Key key) override;
	void OnKeyHold(Key key) override;

	void OnMouseKeyPress(MouseKey key) override;
	void OnMouseKeyRelease(MouseKey key) override;
	void OnMouseMove(MouseMove move) override;
	void OnMouseScroll(MouseScroll scroll) override;

	void AddElement(IGUIElement* elem);
	void RemoveElementAt(int index);
	IGUIElement* GetElementAt(int index);
	const IGUIElement* GetElementAt(int index) const;

	void RenderElements();

	void RenderText(int x, int y, const BitmapFont* font, std::string text, vec4 color = {1,1,1,1});
	void RenderTextClipped(
	    int x, int y,
	    const BitmapFont* font,
	    std::string text,
	    Rect2D clip,
	    vec4 color = {1,1,1,1}
	);
	void RenderRect2D(Rect2D rect);
	void BatchRenderRect2D(Vector<Rect2D> rect);

	void RenderRect2DClipped(Rect2D rect, Rect2D clip);
	void BatchRenderRect2DClipped(Vector<Rect2D> rect, Rect2D clip);

	void SetExtraInputFrame(InputFrame* extra) { extraFrame = extra; }
    private:
	Vector<IGUIElement*> Elements;
	Text text;

	InputFrame* extraFrame = nullptr;

	/// The element, above which the mouse is currently hovering.
	IGUIElement* mousedOver = nullptr;
	/// The element on which the mouse was pressed.
	IGUIElement* clickedOn = nullptr;
	/// The element, which has keyboard focus.
	IGUIElement* keyboardFocus = nullptr;

	GL::VAO vao;
	Camera cam = Camera(OrthographicT());
    };
}

#endif
