#include "GUI.hpp"

#include <algorithm>

#include "../Core/Input.hpp"
#include "../Core/Resources.hpp"

#include "../Utility/Math.hpp"

namespace GUI {
	IElement* currFocused;
	void setCurrentlyFocused(IElement* el) { currFocused = el; }
	bool noCurrFocused() { return currFocused == NULL; }

	// -----------------------------------------------------------
	Image::Image(const Texture* image) {
		img = image;
		fitToImage();
	}
	Image::Image(const Texture* image, int w, int h) : IElement(0, 0, w, h) {
		assert(image != NULL);
		img = image;
		x = 0;
		y = 0;
	}
	void Image::fitToImage() {
		assert(img != NULL);
		w = img->getW();
		h = img->getH();
	}
	const Texture* Image::getTexture() { return img; }
	// -----------------------------------------------------------
	const Texture* Button::getTexture() {
		switch (currState) {
			case Button::State::INACTIVE:
				return inactive;

			case Button::State::ACTIVE:
				if (!active)
					return inactive;
				else
					return active;

			case Button::State::PRESSED:
				return pressed;

			default:
				return inactive;
		}
	}

	void Button::update() {
		bool mousedOver = Input.Mouse.x > x && Input.Mouse.y > y &&
			Input.Mouse.x < x + w && Input.Mouse.y < y + h;
		bool focused = currFocused == this;
		bool noneFocused = noCurrFocused();

		if ((noneFocused || focused) &&	(mousedOver || currState == State::PRESSED) && Input.Mouse.lButton) {
			currState = State::PRESSED;
			setCurrentlyFocused(this);
		} else if (mousedOver) {
			currState = State::ACTIVE;
		} else {
			currState = State::INACTIVE;
		}

		if (currState == State::INACTIVE && focused) {
			setCurrentlyFocused(NULL);
		}
	}
	// -----------------------------------------------------------
	Renderer::Renderer() {
		shad = Resources::GetShader("GUI");

		Vertex v0(glm::vec3(-1,  1, 0), glm::vec2(0, 1), glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
		Vertex v1(glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::normalize(glm::vec3( 0.5,  0.5, 0.5)));
		Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::normalize(glm::vec3( 0.5, -0.5, 0.5)));
		Vertex v3(glm::vec3( 1, -1, 0), glm::vec2(1, 0), glm::normalize(glm::vec3(-0.5,  0.5, 0.5)));

		rect = new Mesh({v0, v1, v2, v3}, {0, 2, 1, 1, 2, 3});

		sortedByLayer = true;
	}

	Renderer::~Renderer() {
		for (Renderer::ElementType ei : elems) {
			if (ei.hasFirst()) {
				delete ei.getFirst();
			} else {
				for (IElement* e : ei.getSecond()->getElements().elements) {
					delete e;
				}
			}
		}

		delete rect;
	}

	void Renderer::add(IElement* elem) {
		sortedByLayer = false;
		elems.push_back(Renderer::ElementType(elem));
	}

	void Renderer::add(IElementContainer* elemCont) {
		sortedByLayer = false;
		elems.push_back(Renderer::ElementType(elemCont));
	}

	void GUI::Renderer::renderElement(IElement* e) {
		if (e->x > Display::GetWidth() ||
			e->y > Display::GetHeight() ||
			e->x + e->w < 0 ||
			e->y + e->h < 0)
			return;

		transform.scale.x = (float)e->w / Display::GetWidth();
		transform.scale.y = (float)e->h / Display::GetHeight();

		transform.pos.x = Util::PixelToGLCoord(Display::GetWidth(), e->x + e->w / 2);
		transform.pos.y = Util::PixelToGLCoord(Display::GetHeight(), Display::GetHeight() - (e->y + e->h / 2));

		shad->setUniformData("transform", transform);

		e->getTexture()->bind();
		e->preRender();
		rect->render();
		e->postRender();
	}

	void GUI::Renderer::render() {
		if (!sortedByLayer) {
			std::sort(elems.begin(), elems.end(), LayerSorter());
			sortedByLayer = true;
		}

		shad->use();

		for (GUI::Renderer::ElementType ei : elems) {
			if (ei.hasFirst()) {
				renderElement(ei.getFirst());
			} else {
				ei.getSecond()->preGroupRender();
				for (IElement* e : ei.getSecond()->getElements().elements) {
					renderElement(e);
				}
				ei.getSecond()->postGroupRender();
			}
		}

		shad->unuse();
	}

	void Renderer::update() {
		for (auto ei : elems) {
			if (ei.hasFirst()) {
				ei.getFirst()->update();
			} else {
				for (IElement* e : ei.getSecond()->getElements().elements) {
					e->update();
				}
			}
		}
	}
	const std::vector<Renderer::ElementType>& Renderer::getElems() const {
		return elems;
	}

	Renderer::ElementType& Renderer::getElem(int index) { return elems[index]; }
	// -----------------------------------------------------------
	void Draggable::update() {
		bool mousedOver =
			Input.Mouse.x > x &&
			Input.Mouse.y > y &&
			Input.Mouse.x < x + w &&
			Input.Mouse.y < y + h;

		if (currFocused == this) { // If being dragged ...
			if (Input.Mouse.lButton) { // ... and the mouse is still being held
				// Set position to mouse.xy - offset.xy
				x = Input.Mouse.x + offsetX;
				y = Input.Mouse.y + offsetY;
			} else { // ... but LMB isn't being pressed anymore
				// Reset the offset
				offsetX = offsetY = 0;

				// Change currFocused object to not be you
				currFocused = nullptr;
			}
		} else if (noCurrFocused()) { // If not being dragged and there are no other active elements ...
			if (mousedOver && Input.Mouse.lButton) { // ... and the mouse is on top of the element with LMB pressed
				// Change currFocused to indicate you are being dragged
				currFocused = this;

				// Set offset to position.xy - mouse.xy
				offsetX = x - Input.Mouse.x;
				offsetY = y - Input.Mouse.y;
			}
		}
	}
}
