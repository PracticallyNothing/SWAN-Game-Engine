#ifndef SWAN_GUI_ELEMENT_HPP
#define SWAN_GUI_ELEMENT_HPP

#include <vector> // For std::vector<T>
#include <string> // For std::string

#include "../Core/EventListener.hpp" // For SWAN::EventListener

#include "../Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "../Rendering/Texture.hpp" // For SWAN::Texture
#include "../Rendering/Image.hpp"   // For SWAN::Color

namespace SWAN {
	struct GUIElement {
		GUIElement(Type t = T_COLOR) : type(t) {}

		GUIElement(int x, int y, int w, int h, Type t = T_COLOR)
			: x(x), y(y), w(w), h(h), type(t) {}

		GUIElement(const GUIElement& other) :
			x(other.x), y(other.y), w(other.w), h(other.h), type(other.type), renderData(other.renderData) {}

		GUIElement(GUIElement&& other) :
			x(std::move(other.x)),
			y(std::move(other.y)),
			w(std::move(other.w)),
			h(std::move(other.h)),
			type(std::move(other.type)),
			renderData(std::move(other.renderData))
		{}

		GUIElement& operator=(const GUIElement& other) {
			x = other.x;
			y = other.y;
			w = other.w;
			h = other.h;
			type = other.type;
			renderData = other.renderData;

			return *this;
		}

		GUIElement& operator=(GUIElement&& other) {
			x = std::move(other.x);
			y = std::move(other.y);
			w = std::move(other.w);
			h = std::move(other.h);
			type = std::move(other.type);
			renderData = std::move(other.renderData);
		}

		int x, y, w, h;

		enum Type {
			T_COLOR,
			T_VERTGRAD,  // Vertical gradient
			T_HORIZGRAD, // Horizontal gradient
			T_TEXTURE
		} type;

		union {
			Color color;
			struct { Color color1, color2 } gradient;
			const Texture* texture;
		} renderData;

		std::vector <EventListener> listeners;

		void update() {
			for (const auto& l : listeners)
				l.update();
		}
	};

	inline GUIElement CreateButton (
			int x, int y,
			int w, int h,
			const Texture* normal,
			const Texture* focused,
			const Texture* active,
			EventListener::ActionFuncT onClick) {
		GUIElement res(x, y, w, h, GUIElement::T_TEXTURE);
		res.renderData.texture = normal;

		//EventListener onMouseOverListener (...);
		//res.listeners.push_back(onMouseOverListener);

		//EventListener onClickListener (...);
		//res.listeners.push_back(onClickListener);

		return res;
	}
}

#endif // SWAN_GUI_ELEMENT_HPP
