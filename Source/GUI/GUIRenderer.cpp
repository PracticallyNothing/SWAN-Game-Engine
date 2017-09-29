#include "GUIRenderer.hpp"

#include "../Core/Display.hpp"   // For Display
#include "../Core/Resources.hpp" // For Resources::GetShader()
#include "../Utility/Math.hpp"   // For Util::PixelToGLCoord

GUIRenderer::GUIRenderer() {
	shad = Resources::GetShader("GUI");

	Vertex v0(glm::vec3(-1,  1, 0), glm::vec2(0, 1), glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
	Vertex v1(glm::vec3( 1,  1, 0), glm::vec2(1, 1), glm::normalize(glm::vec3( 0.5,  0.5, 0.5)));
	Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0), glm::normalize(glm::vec3( 0.5, -0.5, 0.5)));
	Vertex v3(glm::vec3( 1, -1, 0), glm::vec2(1, 0), glm::normalize(glm::vec3(-0.5,  0.5, 0.5)));

	rect = new Mesh({v0, v1, v2, v3}, {0, 2, 1, 1, 2, 3});

	sortedByLayer = true;
}

GUIRenderer::~GUIRenderer() {
	for (GUIRenderer::ElementType ei : elems) {
		if (ei.hasFirst()) {
			delete ei.getFirst();
		} else {
			for (GUIPrim::IElement* e : ei.getSecond()->getElements().elements) {
				delete e;
			}
		}
	}

	delete rect;
}

void GUIRenderer::add(GUIPrim::IElement* elem) {
	sortedByLayer = false;
	elems.push_back(GUIRenderer::ElementType(elem));

}
void GUIRenderer::add(GUIPrim::IElementContainer* elemCont) {
	sortedByLayer = false;
	elems.push_back(GUIRenderer::ElementType(elemCont));
}

void GUIRenderer::renderElement(GUIPrim::IElement* e) {
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

void GUIRenderer::render() {
	if (!sortedByLayer) {
		std::sort(elems.begin(), elems.end(), LayerSorter());
		sortedByLayer = true;
	}

	shad->use();

	for (GUIRenderer::ElementType ei : elems) {
		if (ei.hasFirst()) {
			renderElement(ei.getFirst());
		} else {
			ei.getSecond()->preGroupRender();
			for (GUIPrim::IElement* e : ei.getSecond()->getElements().elements) {
				renderElement(e);
			}
			ei.getSecond()->postGroupRender();
		}
	}

	shad->unuse();
}

void GUIRenderer::update() {
	for (auto ei : elems) {
		if (ei.hasFirst()) {
			ei.getFirst()->update();
		} else {
			for (GUIPrim::IElement* e : ei.getSecond()->getElements().elements) {
				e->update();
			}
		}
	}
}
const std::vector<GUIRenderer::ElementType>&
GUIRenderer::getElems() const {
	return elems;
}

GUIRenderer::ElementType& GUIRenderer::getElem(int index) { return elems[index]; }
