#include "GUI.hpp"

#include <algorithm>
#include "../Core/Input.hpp"
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
		//TODO: Load shader with Resources::GetShader()
		shad = new Shader();
		shad->compileShaders("Resources/Shaders/GUIShader.vert",
							 "Resources/Shaders/GUIShader.frag");
		{
			shad->addAttrib("pos");
			shad->addAttrib("UV");
			shad->addAttrib("norm");
		}
		shad->linkShaders();

		shad->use();
		{
			shad->addUniform("transform");
		}
		shad->unuse();

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
		delete shad;
	}

	void Renderer::add(IElement* elem) {
		sortedByLayer = false;
		elems.push_back(Renderer::ElementType(elem));
	}

	void Renderer::add(IElementContainer* elemCont) {
		sortedByLayer = false;
		elems.push_back(Renderer::ElementType(elemCont));
	}

	void GUI::Renderer::renderElement(IElement* e, Display& d) {
		if (e->x > d.getW() ||
			e->y > d.getH() ||
			e->x + e->w < 0 ||
			e->y + e->h < 0)
			return;

		transform.scale.x = (double)e->w / d.getW();
		transform.scale.y = (double)e->h / d.getH();

		transform.pos.x = Util::pixelToGLCoord(d.getW(), e->x + e->w / 2);
		transform.pos.y = Util::pixelToGLCoord(d.getH(), d.getH() - (e->y + e->h / 2));

		shad->setUniformData("transform", transform);

		e->getTexture()->bind();
		e->preRender();
		rect->render();
		e->postRender();
	}

	void GUI::Renderer::render(Display& d) {
		if (!sortedByLayer) {
			std::sort(elems.begin(), elems.end(), LayerSorter());
			sortedByLayer = true;
		}

		shad->use();

		for (GUI::Renderer::ElementType ei : elems) {
			if (ei.hasFirst()) {
				renderElement(ei.getFirst(), d);
			} else {
				ei.getSecond()->preGroupRender();
				for (IElement* e : ei.getSecond()->getElements().elements) {
					renderElement(e, d);
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

		if ((currFocused == this || noCurrFocused()) && mousedOver && Input.Mouse.lButton) {
			setCurrentlyFocused(this);
			moving = true;
				
			int relMouseX = Input.Mouse.x - x, 
				relMouseY = Input.Mouse.y - y;

			if (lastMouseX < 0)
				lastMouseX = relMouseX;
			if (lastMouseY < 0)
				lastMouseY = relMouseY;

			x += (relMouseX - lastMouseX) ;
			y += (relMouseY - lastMouseY) ;

			lastMouseX = relMouseX;
			lastMouseY = relMouseY;
		} else {
			if (currFocused == this) {
				setCurrentlyFocused(NULL);
			}

			lastMouseX = -1;
			lastMouseY = -1;
		}
	}
}
