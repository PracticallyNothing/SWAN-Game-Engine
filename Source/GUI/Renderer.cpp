#include "Renderer.hpp"

#include "Core/Display.hpp"   // For Display
#include "Core/Resources.hpp" // For SWAN::Res::GetShader()
#include "Utility/Debug.hpp"  // For SWAN_DEBUG_PRINT()
#include "Utility/Math.hpp"   // For SWAN::Util::PixelToGLCoord

#include <cassert> // For assert()

namespace SWAN {
namespace GUI {
	using namespace Util::StreamOps;
	Renderer::Renderer() {
		initMesh();
		sortedByLayer = true;
	}

	Renderer::Renderer(Shader* s) {
		setShader(s);
		initMesh();
		sortedByLayer = true;
	}

	void Renderer::initMesh() {
		SWAN::Vertex v0(glm::vec3(-1, 1, 0), glm::vec2(0, 1),
		                glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));
		SWAN::Vertex v1(glm::vec3(1, 1, 0), glm::vec2(1, 1),
		                glm::normalize(glm::vec3(0.5, 0.5, 0.5)));
		SWAN::Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0),
		                glm::normalize(glm::vec3(0.5, -0.5, 0.5)));
		SWAN::Vertex v3(glm::vec3(1, -1, 0), glm::vec2(1, 0),
		                glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

		rect = std::unique_ptr<Mesh>(new SWAN::Mesh({ v0, v1, v2, v3 }, { 0, 2, 1, 1, 2, 3 }));
	}

	Element* Renderer::add(std::unique_ptr<Element> e) {
		//sortedByLayer = false;
		elems.emplace_back(std::move(e));
		return e.get();
	}

	void Renderer::renderElement(const Element* el) {
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
			case Element::T_COLOR:
				unis.emplace_back("color1", (glm::vec4) el->renderData.color);
				break;
			case Element::T_VERTGRAD:
			case Element::T_HORIZGRAD:
				unis.emplace_back("color1", (glm::vec4) el->renderData.gradient.color1);
				unis.emplace_back("color2", (glm::vec4) el->renderData.gradient.color2);
				unis.emplace_back("topLeft", transform.getModel() * glm::vec4(-1, 1, 0, 1));
				unis.emplace_back("bottomRight", transform.getModel() * glm::vec4(1, -1, 0, 1));
				break;
			case Element::T_TEXTURE:
				unis.emplace_back("tex", el->renderData.texture);
				break;
		}

		shad->setUniforms(unis);
		shad->renderMesh(*rect);

		if(el->hasChildren())
			for(auto& c : el->children)
				renderElement(c.get());
	}

	void Renderer::render() {
		if(!elems.empty())
			assert(shad != nullptr);

		for(auto& el : elems) {
			renderElement(el.get());
		}
	}
	void Renderer::update() {
		for(auto& e : elems) {
			e->update();
		}
	}
} // namespace GUI
} // namespace SWAN
