#include "Renderer.hpp"

#include "Core/Display.hpp"   // For Display
#include "Core/Resources.hpp" // For SWAN::Res::GetShader()
#include "Utility/Debug.hpp"  // For SWAN_DEBUG_PRINT()
#include "Utility/Math.hpp"   // For SWAN::Util::PixelToGLCoord
#include "Maths/Vector.hpp" // For SWAN::vec2

#include <array>   // For std::array<T, N>
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
		std::array<vec2, 4>
		    pos = { vec2(-1, -1), vec2(-1, 1), vec2(1, 1), vec2(1, -1) },
		    UVs = { vec2(0, 0),   vec2(0, 1),  vec2(1, 1), vec2(1, 0) };
		vao.bind();
		vao.storeAttribData(0, 2, (float*) pos.data(), pos.size() * sizeof(vec2));
		vao.storeAttribData(1, 2, (float*) UVs.data(), UVs.size() * sizeof(vec2));
		vao.unbind();
	}

	Element* Renderer::add(std::unique_ptr<Element> e) {
		//sortedByLayer = false;
		elems.emplace_back(std::move(e));
		return elems.back().get();
	}

	void Renderer::renderElement(const Element* el) {
		if(!el->isVisible())
			return;

		if(el->x > Display::GetWidth() || el->y > Display::GetHeight() ||
		   el->x + el->w < 0 || el->y + el->h < 0)
			return;

		std::vector<ShaderUniform> unis;

		Transform transform;
		transform.scale.x = (float) el->w / Display::GetWidth();
		transform.scale.y = (float) el->h / Display::GetHeight();

		transform.pos.x = SWAN::Util::PixelToGLCoord(Display::GetWidth(), el->x + el->w / 2);
		transform.pos.y = SWAN::Util::PixelToGLCoord(Display::GetHeight(), Display::GetHeight() - (el->y + el->h / 2));

		unis.emplace_back("type", el->type);
		unis.emplace_back("transform", transform);

		switch(el->type) {
			case Element::T_COLOR:
				unis.emplace_back("color1", (vec4) el->renderData.color);
				break;
			case Element::T_VERTGRAD:
			case Element::T_HORIZGRAD:
				unis.emplace_back("color1", (vec4) el->renderData.gradient.color1);
				unis.emplace_back("color2", (vec4) el->renderData.gradient.color2);
				unis.emplace_back("topLeft", vec4(-1, 1, 0, 1) * transform.getModel());
				unis.emplace_back("bottomRight", vec4(1, -1, 0, 1) * transform.getModel());
				break;
			case Element::T_TEXTURE:
				unis.emplace_back("tex", el->renderData.texture);
				break;
		}

		shad->setUniforms(unis);
		shad->use();
		vao.draw(4, GL_TRIANGLE_FAN);
		shad->unuse();

		if(el->hasChildren())
			for(auto& c : el->children)
				renderElement(c.get());
	}

	void Renderer::render() {
		if(!elems.empty())
			assert(shad != nullptr);

		vao.bind();
		for(auto& el : elems) {
			renderElement(el.get());
		}
	}
	void Renderer::update() {
		for(auto& e : elems) {
			if(e->isVisible())
				e->update();
		}
	}
} // namespace GUI
} // namespace SWAN
