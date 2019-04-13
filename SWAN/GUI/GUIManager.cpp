#include "GUIManager.hpp"

#include "Core/Display.hpp"
#include "Core/Format.hpp"
#include "Core/Logging.hpp"
#include "Maths/Vector.hpp"
#include "OpenGL/OnGLInit.hpp"
#include "Rendering/Shader.hpp"

static const char* GUIVertSrc = R"ddd(
#version 130

in vec2 pos;
in vec4 color;

out vec4 col;

uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * vec4(pos.x, pos.y, 0, 1);
    col = color;
}
)ddd";

static const char* GUIFragSrc = R"ddd(
#version 130

in vec4 col;
out vec4 fCol;

void main() {
    fCol = col;
}
)ddd";

static SWAN::Shader GUIShader;
static SWAN::OnGLInit _ = {
	[] {
	    GUIShader.compileShadersFromSrc(GUIVertSrc, GUIFragSrc);
	    GUIShader.addAttrib("pos");
	    GUIShader.addAttrib("color");
	    GUIShader.linkShaders();

	    GUIShader.use();
	    GUIShader.addUniform("viewProj");
	}
};

namespace SWAN
{
	void GUIManager::OnWindowExit()
	{
		if(extraFrame)
			extraFrame->OnWindowExit();
		else
			Display::Close();
	}

	void GUIManager::OnKeyPress(Key key)
	{
		bool eventConsumed = false;
		if(keyboardFocus)
			eventConsumed = keyboardFocus->OnKeyPress(key);
		if(!eventConsumed && extraFrame)
			extraFrame->OnKeyPress(key);
	}
	void GUIManager::OnKeyRepeat(Key key)
	{
		bool eventConsumed = false;
		if(keyboardFocus)
			eventConsumed = keyboardFocus->OnKeyRepeat(key);
		if(!eventConsumed && extraFrame)
			extraFrame->OnKeyRepeat(key);
	}
	void GUIManager::OnKeyHold(Key key)
	{
		bool eventConsumed = false;
		if(keyboardFocus)
			eventConsumed = keyboardFocus->OnKeyHold(key);
		if(!eventConsumed && extraFrame)
			extraFrame->OnKeyHold(key);
	}
	void GUIManager::OnKeyRelease(Key key)
	{
		bool eventConsumed = false;
		if(keyboardFocus)
			eventConsumed = keyboardFocus->OnKeyRelease(key);
		if(!eventConsumed && extraFrame)
			extraFrame->OnKeyRelease(key);
	}
	void GUIManager::OnMouseKeyPress(MouseKey key)
	{
		bool eventConsumed = false;

		if(!mousedOver) { // If not hovering over anything...
			if(keyboardFocus) {
				keyboardFocus->OnLoseFocus();
				keyboardFocus = nullptr;
			}
		} else if(!keyboardFocus) { // If there is an element, but no keyboard focus...
			keyboardFocus = mousedOver;
			keyboardFocus->OnGainFocus();
			eventConsumed = keyboardFocus->OnMouseKeyPress(key);
		} else if(keyboardFocus != mousedOver) { // If there is a keyboard focus, but it's not being moused over...
			keyboardFocus->OnLoseFocus();
			keyboardFocus = mousedOver;
			keyboardFocus->OnGainFocus();
			eventConsumed = keyboardFocus->OnMouseKeyPress(key);
		} else { // Else if the keyboard focus and the element being moused over are the same...
			eventConsumed = keyboardFocus->OnMouseKeyPress(key);
		}

		if(!eventConsumed && extraFrame)
			extraFrame->OnMouseKeyPress(key);
	}

	void GUIManager::OnMouseKeyRelease(MouseKey key)
	{
		bool eventConsumed = false;

		if(mousedOver) {
			//clickedOn = mousedOver;
			eventConsumed = mousedOver->OnMouseKeyRelease(key);
		}

		if(!eventConsumed && extraFrame)
			extraFrame->OnMouseKeyRelease(key);
	}

	void GUIManager::OnMouseMove(MouseMove move)
	{
		IGUIElement* target = nullptr;

		if(!clickedOn) {
			for(auto it = Elements.cbegin(); it < Elements.cend(); it++) {
				auto r = (*it)->GetRect();
				if(r.IsPointInside(ivec2(move.X, move.Y)))
					target = *it;
			}
		} else {
			target = clickedOn;
		}

		bool eventConsumed = false;
		if(!mousedOver && !target) {
		} else if(!mousedOver && target) {
			target->OnMouseEnter(move);
			mousedOver = target;
			eventConsumed = true;
		} else if(mousedOver && !target) {
			mousedOver->OnMouseLeave(move);
			mousedOver = nullptr;
			eventConsumed = false;
		} else if(mousedOver != target) {
			mousedOver->OnMouseLeave(move);
			target->OnMouseEnter(move);
			mousedOver = target;
			eventConsumed = true;
		} else {
			eventConsumed = mousedOver->OnMouseMoveInside(move);
		}

		if(!eventConsumed && extraFrame)
			extraFrame->OnMouseMove(move);
	}

	void GUIManager::OnMouseScroll(MouseScroll scroll)
	{
		bool eventConsumed = false;
		if(mousedOver)
			eventConsumed = mousedOver->OnMouseScroll(scroll);
		if(!eventConsumed && extraFrame)
			extraFrame->OnMouseScroll(scroll);
	}

	void GUIManager::AddElement(IGUIElement* elem) { Elements.push_back(elem); }
	IGUIElement* GUIManager::GetElementAt(int index) { return Elements.at(index); }
	const IGUIElement* GUIManager::GetElementAt(int index) const { return Elements.at(index); }
	void GUIManager::RemoveElementAt(int index) { Elements.erase(Elements.begin() + index); }

	void GUIManager::RenderElements()
	{
		glDisable(GL_DEPTH_TEST);
		for(auto* elem : Elements)
			elem->OnRender(*this);
		glEnable(GL_DEPTH_TEST);
	}

	void GUIManager::RenderText(int x, int y,
	                            const BitmapFont* font, std::string text,
	                            vec4 color)
	{
		this->text.font = font;
		this->text = text;
		this->text.render(x, y, color);
	}

	void GUIManager::RenderTextClipped(int x, int y,
	                                   const BitmapFont* font, std::string text,
	                                   Rect2D clip, vec4 color)
	{
		this->text.font = font;
		this->text = text;
		this->text.RenderClipped(x, y, clip, color);
	}

	void GUIManager::SetClipArea(Rect2D clip)
	{
		glScissor(clip.Position.x,
		          Display::GetHeight() - (clip.Position + clip.Size).y,
		          clip.Size.x,
		          clip.Size.y);
	}

	void GUIManager::ClearClipArea()
	{
		glScissor(0, 0, Display::GetWidth(), Display::GetHeight());
	}

	void GUIManager::RenderRect2D(Rect2D rect)
	{
		BatchRenderRect2D({ rect });
	}

	void GUIManager::RenderRect2DClipped(Rect2D rect, Rect2D clip)
	{
		rect.ClipAgainst(clip);
		RenderRect2D(rect);
	}

	void GUIManager::BatchRenderRect2DClipped(Vector<Rect2D> rects, Rect2D clip)
	{
		for(Rect2D& rect : rects)
			rect.ClipAgainst(clip);

		SetClipArea(clip);
		BatchRenderRect2D(rects);
		ClearClipArea();
	}

	void GUIManager::BatchRenderRect2D(Vector<Rect2D> rects)
	{
		if(rects.size() < 1)
			return;

		Vector<fvec2> points;
		Vector<fvec4> colors;
		Vector<unsigned> indices;

		int i = 0;
		for(Rect2D r : rects) {
			if(r.Size.x == 0 || r.Size.y == 0)
				continue;

			fvec2 p0(r.Position),
			    p1(r.Position + fvec2(0, r.Size.y)),
			    p2(r.Position + fvec2(r.Size.x, 0)),
			    p3(r.Position + fvec2(r.Size.x, r.Size.y));

			points.insert(points.end(), { p0, p1, p2, p3 });
			colors.insert(colors.end(), { r.Colors[0], r.Colors[1],
			                              r.Colors[2], r.Colors[3] });
			unsigned j = i; // Can't convert to unsigned inside of initializer,
			                // so this is the next best solution.
			indices.insert(indices.end(), { j * 4 + 0, j * 4 + 1, j * 4 + 2,
			                                j * 4 + 2, j * 4 + 1, j * 4 + 3 });
			i++;
		}

		vao.bind();
		vao.storeAttribData(0, 2, (float*) points.data(), points.size() * sizeof(fvec2));
		vao.storeAttribData(1, 4, (float*) colors.data(), colors.size() * sizeof(fvec4));
		vao.storeIndices(indices.data(), indices.size() * sizeof(unsigned));

		GUIShader.use();
		GUIShader.SetMat4("viewProj", cam.getPerspective() * cam.getView());
		vao.draw(rects.size() * 6, GL_TRIANGLES);
	}
} // namespace SWAN
