#include "GUIRenderer.hpp"

#include "Core/Display.hpp"   // For Display
#include "Core/Resources.hpp" // For SWAN::Res::GetShader()
#include "Utility/Debug.hpp"  // For SWAN_DEBUG_PRINT()
#include "Utility/Math.hpp"   // For SWAN::Util::PixelToGLCoord
#include "Utility/StreamOps.hpp"

#include <cassert> // For assert()

namespace SWAN {
using namespace Util::StreamOps;
GUIRenderer::GUIRenderer() {
	shad = SWAN::Res::GetShader("GUI");

	SWAN::Vertex v0(glm::vec3(-1, 1, 0), glm::vec2(0, 1),
	                glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
	SWAN::Vertex v1(glm::vec3(1, 1, 0), glm::vec2(1, 1),
	                glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
	SWAN::Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0),
	                glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
	SWAN::Vertex v3(glm::vec3(1, -1, 0), glm::vec2(1, 0),
	                glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

	rect = new SWAN::Mesh({ v0, v1, v2, v3 }, { 0, 2, 1, 1, 2, 3 });

	sortedByLayer = true;
}

GUIRenderer::~GUIRenderer() {
	for(GUIRenderer::ElementType ei : elems) {
		if(ei.hasFirst()) {
			delete ei.getFirst();
		} else {
			for(GUIP::IElement* e : ei.getSecond()->getElements().elements) {
				delete e;
			}
		}
	}

	delete rect;
}

GUIP::IElement* GUIRenderer::add(GUIP::IElement* elem) {
	sortedByLayer = false;
	elems.push_back(GUIRenderer::ElementType(elem));
	return elem;
}

GUIP::IElementContainer* GUIRenderer::add(GUIP::IElementContainer* elemCont) {
	sortedByLayer = false;
	elems.push_back(GUIRenderer::ElementType(elemCont));
	return elemCont;
}

void GUIRenderer::renderElement(GUIP::IElement* e) {
	if(e->x > Display::GetWidth() || e->y > Display::GetHeight() ||
	   e->x + e->w < 0 || e->y + e->h < 0)
		return;

	transform.scale.x = (float) e->w / Display::GetWidth();
	transform.scale.y = (float) e->h / Display::GetHeight();

	transform.pos.x =
	    SWAN::Util::PixelToGLCoord(Display::GetWidth(), e->x + e->w / 2);
	transform.pos.y = SWAN::Util::PixelToGLCoord(
	    Display::GetHeight(), Display::GetHeight() - (e->y + e->h / 2));

	shad->setUniform({ "transform", transform });

	e->getTexture()->bind();
	e->preRender();
	shad->renderMesh(*rect);
	e->postRender();
}

void GUIRenderer::render() {
	if(!sortedByLayer) {
		std::sort(elems.begin(), elems.end(), LayerSorter());
		sortedByLayer = true;
	}

	for(GUIRenderer::ElementType ei : elems) {
		if(ei.hasFirst()) {
			renderElement(ei.getFirst());
		} else {
			ei.getSecond()->preGroupRender();
			for(GUIP::IElement* e : ei.getSecond()->getElements().elements) {
				renderElement(e);
			}
			ei.getSecond()->postGroupRender();
		}
	}
}

void GUIRenderer::update() {
	for(auto ei : elems) {
		if(ei.hasFirst()) {
			ei.getFirst()->update();
		} else {
			for(GUIP::IElement* e : ei.getSecond()->getElements().elements) {
				e->update();
			}
		}
	}
}
const std::vector<GUIRenderer::ElementType>& GUIRenderer::getElems() const {
	return elems;
}

GUIRenderer::ElementType& GUIRenderer::getElem(int index) {
	return elems[index];
}

GUIElement* GUIRenderer::_exp_add(std::unique_ptr<GUIElement> e) {
	//sortedByLayer = false;
	_exp_elems.emplace_back(std::move(e));
	return e.get();
}

void GUIRenderer::_exp_renderElement(GUIElement* el) {
	if(el->x > Display::GetWidth() || el->y > Display::GetHeight() ||
	   el->x + el->w < 0 || el->y + el->h < 0)
		return;

	std::vector<ShaderUniform> unis;

	Transform transform;
	transform.scale.x = (float) el->w / Display::GetWidth();
	transform.scale.y = (float) el->h / Display::GetHeight();

	transform.pos.x =
	    SWAN::Util::PixelToGLCoord(Display::GetWidth(), el->x + el->w / 2);
	transform.pos.y = SWAN::Util::PixelToGLCoord(
	    Display::GetHeight(), Display::GetHeight() - (el->y + el->h / 2));

	unis.emplace_back("type", el->type);
	unis.emplace_back("transform", transform);

	switch(el->type) {
		case GUIElement::T_COLOR:
			unis.emplace_back("color1", (glm::vec4) el->renderData.color);
			break;
		case GUIElement::T_VERTGRAD:
		case GUIElement::T_HORIZGRAD:
			unis.emplace_back("color1", (glm::vec4) el->renderData.gradient.color1);
			unis.emplace_back("color2", (glm::vec4) el->renderData.gradient.color2);
			unis.emplace_back("topLeft", transform.getModel() * glm::vec4(-1, 1, 0, 1));
			unis.emplace_back("bottomRight", transform.getModel() * glm::vec4(1, -1, 0, 1));
			break;
		case GUIElement::T_TEXTURE:
			unis.emplace_back("tex", el->renderData.texture);
			break;
	}

	_exp_shad->setUniforms(unis);
	_exp_shad->renderMesh(*rect);

	if(el->hasChildren())
		for(auto& c : el->children)
			_exp_renderElement(c.get());
}

void GUIRenderer::_exp_render() {
	assert(_exp_shad != nullptr);

	//assert(_exp_shad->hasUniform("transform"));

	//assert(_exp_shad->hasUniform("topLeft"));
	//assert(_exp_shad->hasUniform("bottomRight"));

	//assert(_exp_shad->hasUniform("color1"));
	//assert(_exp_shad->hasUniform("color2"));
	//assert(_exp_shad->hasUniform("tex"));
	//assert(_exp_shad->hasUniform("type"));

	for(auto& el : _exp_elems) {
		_exp_renderElement(el.get());
	}
}
void GUIRenderer::_exp_update() {
	for(auto& e : _exp_elems) {
		e->update();
	}
}
}
