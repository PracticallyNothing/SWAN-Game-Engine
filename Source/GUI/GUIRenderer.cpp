#include "GUIRenderer.hpp"

#include "Core/Display.hpp"   // For Display
#include "Core/Resources.hpp" // For SWAN::Res::GetShader()
#include "Utility/Math.hpp"   // For SWAN::Util::PixelToGLCoord

namespace SWAN {
GUIRenderer::GUIRenderer () {
	shad = SWAN::Res::GetShader ("GUI");

	SWAN::Vertex v0 (glm::vec3 (-1, 1, 0), glm::vec2 (0, 1), glm::normalize (glm::vec3 (-0.5, -0.5, 0.5)));
	SWAN::Vertex v1 (glm::vec3 (1, 1, 0), glm::vec2 (1, 1), glm::normalize (glm::vec3 (0.5, 0.5, 0.5)));
	SWAN::Vertex v2 (glm::vec3 (-1, -1, 0), glm::vec2 (0, 0), glm::normalize (glm::vec3 (0.5, -0.5, 0.5)));
	SWAN::Vertex v3 (glm::vec3 (1, -1, 0), glm::vec2 (1, 0), glm::normalize (glm::vec3 (-0.5, 0.5, 0.5)));

	rect = new SWAN::Mesh ({ v0, v1, v2, v3 }, { 0, 2, 1, 1, 2, 3 });

	sortedByLayer = true;
}

GUIRenderer::~GUIRenderer () {
	for (GUIRenderer::ElementType ei : elems) {
		if (ei.hasFirst ()) {
			delete ei.getFirst ();
		} else {
			for (GUIP::IElement* e : ei.getSecond ()->getElements ().elements) {
				delete e;
			}
		}
	}

	delete rect;
}

GUIP::IElement*
GUIRenderer::add (GUIP::IElement* elem) {
	sortedByLayer = false;
	elems.push_back (GUIRenderer::ElementType (elem));
	return elem;
}

GUIP::IElementContainer*
GUIRenderer::add (GUIP::IElementContainer* elemCont) {
	sortedByLayer = false;
	elems.push_back (GUIRenderer::ElementType (elemCont));
	return elemCont;
}

void
GUIRenderer::renderElement (GUIP::IElement* e) {
	if (e->x > Display::GetWidth () ||
	    e->y > Display::GetHeight () ||
	    e->x + e->w < 0 ||
	    e->y + e->h < 0)
		return;

	transform.scale.x = (float) e->w / Display::GetWidth ();
	transform.scale.y = (float) e->h / Display::GetHeight ();

	transform.pos.x = SWAN::Util::PixelToGLCoord (Display::GetWidth (), e->x + e->w / 2);
	transform.pos.y = SWAN::Util::PixelToGLCoord (Display::GetHeight (), Display::GetHeight () - (e->y + e->h / 2));

	shad->setUniform ({ "transform", transform });

	e->getTexture ()->bind ();
	e->preRender ();
	shad->renderMesh (*rect);
	e->postRender ();
}

void
GUIRenderer::render () {
	if (!sortedByLayer) {
		std::sort (elems.begin (), elems.end (), LayerSorter ());
		sortedByLayer = true;
	}

	for (GUIRenderer::ElementType ei : elems) {
		if (ei.hasFirst ()) {
			renderElement (ei.getFirst ());
		} else {
			ei.getSecond ()->preGroupRender ();
			for (GUIP::IElement* e : ei.getSecond ()->getElements ().elements) {
				renderElement (e);
			}
			ei.getSecond ()->postGroupRender ();
		}
	}
}

void
GUIRenderer::update () {
	for (auto ei : elems) {
		if (ei.hasFirst ()) {
			ei.getFirst ()->update ();
		} else {
			for (GUIP::IElement* e : ei.getSecond ()->getElements ().elements) {
				e->update ();
			}
		}
	}
}
const std::vector<GUIRenderer::ElementType>&
GUIRenderer::getElems () const {
	return elems;
}

GUIRenderer::ElementType&
GUIRenderer::getElem (int index) { return elems[index]; }
}
