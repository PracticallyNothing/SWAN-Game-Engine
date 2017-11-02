#ifndef SWAN_GUI_ELEMENT_HPP
#define SWAN_GUI_ELEMENT_HPP

#include "../Core/EventListener.hpp" // For SWAN::EventListener
#include "../Core/Input.hpp"         // For SWAN_Input
#include <memory>                    // For std::unique_ptr<T>
#include <string>                    // For std::string
#include <vector>                    // For std::vector<T>

#include "../Rendering/Image.hpp"   // For SWAN::Color
#include "../Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "../Rendering/Texture.hpp" // For SWAN::Texture

namespace SWAN {
namespace GUI {
	struct Element {
		enum Type {
			T_COLOR = 0,
			T_VERTGRAD,  // Vertical gradient
			T_HORIZGRAD, // Horizontal gradient
			T_TEXTURE,
		} type;

		union {
			Color color;
			struct {
				Color color1, color2;
			} gradient;
			const Texture* texture;
		} renderData;

		explicit Element(Type t = T_COLOR) : type(t) {}

		Element(int x, int y, int w, int h, Type t = T_COLOR)
		    : x(x), y(y), w(w), h(h), type(t) {}

		Element(const Element& other)
		    : x(other.x), y(other.y), w(other.w), h(other.h), type(other.type),
		      renderData(other.renderData) {}

		Element(Element&& other)
		    : x(std::move(other.x)), y(std::move(other.y)), w(std::move(other.w)),
		      h(std::move(other.h)), type(std::move(other.type)),
		      renderData(std::move(other.renderData)) {}

		Element& operator=(const Element& other) {
			x          = other.x;
			y          = other.y;
			w          = other.w;
			h          = other.h;
			type       = other.type;
			renderData = other.renderData;

			return *this;
		}

		Element& operator=(Element&& other) {
			x          = std::move(other.x);
			y          = std::move(other.y);
			w          = std::move(other.w);
			h          = std::move(other.h);
			type       = std::move(other.type);
			renderData = std::move(other.renderData);

			return *this;
		}

		Element& setX(int _x) {
			x = _x;
			return *this;
		}
		Element& setY(int _y) {
			y = _y;
			return *this;
		}

		Element& setXY(int _x, int _y) {
			setX(_x);
			setY(_y);
			return *this;
		}

		Element& setW(int _w) {
			w = _w;
			return *this;
		}
		Element& setH(int _w) {
			h = _h;
			return *this;
		}
		Element& setWH(int _w, int _h) {
			setW(_w);
			setH(_h);
			return *this;
		}

		Element& setLayer(int _layer) {
			layer = _layer;
			return *this;
		}
		Element& setSublayer(int _sublayer) {
			sublayer = _sublayer;
			return *this;
		}
		Element& setLayerSublayer(int _layer, int _sublayer) {
			setLayer(_layer);
			setSublayer(_sublayer);
			return *this;
		}

		Element& setVisible(bool _visible) {
			visible = _visible;
			return *this;
		}
		Element& setParent(const Element* _parent) {
			parent = _parent;
			return *this;
		}

		int x, y, w, h;
		int layer = 0, sublayer = 0;
		bool visible = true;

		std::vector<EventListener> listeners;

		const Element* parent = nullptr;
		std::vector<std::unique_ptr<Element>> children;

		bool hasChildren() const { return !children.empty(); }
		bool isFocused() const {
			if(!focused)
				return false;

			if(focused == this)
				return true;
			else if(parent)
				return parent->isFocused();
			else
				return false;
		}

		bool isVisible() const {
			if(parent)
				return parent->isVisible();
			else
				return visible;
		}

		void update() {
			for(auto& l : listeners)
				l();

			for(auto& c : children)
				c->update();
		}

		static const Element* focused;
	};

	extern std::unique_ptr<Element> CreateImage(
	    int x, int y,
	    int w, int h,
	    const Texture* texture);

	extern std::unique_ptr<Element> CreateButton(
	    int x, int y,
	    int w, int h,
	    const Texture* normal,
	    const Texture* focused,
	    const Texture* active,
	    EventListener::ActionFuncT onRelease);

	extern std::unique_ptr<Element> CreateSlider(
	    int x, int y,
	    int w, int h,
	    Color bgColor,
	    Color activeColor,
	    Color handleColor,
	    std::function<void(double)> onChange);

	extern std::unique_ptr<Element> CreateVerticalSlider(
	    int x, int y,
	    int w, int h,
	    Color bgColor,
	    Color activeColor,
	    Color handleColor,
	    std::function<void(double)> onChange);

	extern std::unique_ptr<Element> CreateCheckBox(
	    int x, int y,
	    int w, int h,
	    const Texture* normal,
	    const Texture* active,
	    std::function<void(bool)> onChange);

} // namespace GUI
} // namespace SWAN

#endif // SWAN_GUI_ELEMENT_HPP
