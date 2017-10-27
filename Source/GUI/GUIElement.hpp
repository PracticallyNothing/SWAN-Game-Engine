#ifndef SWAN_GUI_ELEMENT_HPP
#define SWAN_GUI_ELEMENT_HPP

#include <memory> // For std::unique_ptr<T>
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "../Core/EventListener.hpp" // For SWAN::EventListener
#include "../Core/Input.hpp"         // For SWAN_Input

#include "../Rendering/Image.hpp"   // For SWAN::Color
#include "../Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "../Rendering/Texture.hpp" // For SWAN::Texture

namespace SWAN {
struct GUIElement {
	explicit GUIElement (Type t = T_COLOR)
	  : type (t) {}

	GUIElement (int x, int y, int w, int h, Type t = T_COLOR)
	  : x (x)
	  , y (y)
	  , w (w)
	  , h (h)
	  , type (t) {}

	GUIElement (const GUIElement& other)
	  : x (other.x)
	  , y (other.y)
	  , w (other.w)
	  , h (other.h)
	  , type (other.type)
	  , renderData (other.renderData) {}

	GUIElement (GUIElement&& other)
	  : x (std::move (other.x))
	  , y (std::move (other.y))
	  , w (std::move (other.w))
	  , h (std::move (other.h))
	  , type (std::move (other.type))
	  , renderData (std::move (other.renderData)) {}

	GUIElement& operator= (const GUIElement& other) {
		x          = other.x;
		y          = other.y;
		w          = other.w;
		h          = other.h;
		type       = other.type;
		renderData = other.renderData;

		return *this;
	}

	GUIElement& operator= (GUIElement&& other) {
		x          = std::move (other.x);
		y          = std::move (other.y);
		w          = std::move (other.w);
		h          = std::move (other.h);
		type       = std::move (other.type);
		renderData = std::move (other.renderData);

		return *this;
	}

	int x, y, w, h;
	int layer = 0, sublayer = 0;

	enum Type {
		T_COLOR,
		T_VERTGRAD,  // Vertical gradient
		T_HORIZGRAD, // Horizontal gradient
		T_TEXTURE
	} type;

	union {
		Color color;
		struct {
			Color color1, color2
		} gradient;
		//const Texture* texture;
	} renderData;

	std::vector<EventListener> listeners;
	GUIElement*                parent = nullptr;

	bool parentFocused () {
		if (!parent)
			return focused == this;
		else
			return parent->parentFocused ();
	}

	void update () {
		for (const auto& l : listeners)
			l.update ();
	}

	static const GUIElement* focused;
};

extern std::unique_ptr<GUIElement>
CreateImage (
  int            x,
  int            y,
  int            w,
  int            h,
  const Texture* texture);

extern std::unique_ptr<GUIElement>
CreateButton (
  int                        x,
  int                        y,
  int                        w,
  int                        h,
  const Texture*             normal,
  const Texture*             focused,
  const Texture*             active,
  EventListener::ActionFuncT onRelease);

/*
	extern std::unique_ptr<GUIElement> CreateSlider (
		int x, int y,
		int w, int h,
		Color bgColor,
		Color activeColor,
		Color handleColor,
		EventListener::ActionFuncT onChange
	);
	*/
}

#endif // SWAN_GUI_ELEMENT_HPP
