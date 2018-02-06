#include "Text.hpp"

#include "Core/Display.hpp" // For SWAN::Display::GetWidth(), SWAN::Display::GetHeight()

#include "Utility/Debug.hpp"
#include "Utility/Math.hpp"       // For SWAN::Util::Normalize()
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

using namespace SWAN::Util;

static double calcGLX(int x) { return 2.0 * x / SWAN::Display::GetWidth() - 1.0; };
static double calcGLY(int y) { return -2.0 * y / SWAN::Display::GetHeight() + 1.0; };

namespace SWAN {
void Text::updateVAO() {
	std::vector<glm::vec2> pos, UVs;

	pos.reserve(text.length());
	UVs.reserve(text.length());

	glm::vec2 cursorPos;

	for(int i = 0; i < text.length(); i++) {
		if(text[i] == '\n') {
			cursorPos.x = 0;
			cursorPos.y += font->getGlyphHeight();
			continue;
		} else if(text[i] == ' ') {
			cursorPos.x += font->getGlyphWidth();
			continue;
		} else if(text[i] == '\t') {
			cursorPos.x += font->getGlyphWidth() * font->tabWidth;
			continue;
		}

		char c   = text[i];
		auto uvs = font->getGlyphUVs(c);
		int gw   = font->getGlyphWidth(c),
		    gh   = font->getGlyphHeight();

		glm::vec2
		    p0(calcGLX(cursorPos.x), calcGLY(cursorPos.y)),
		    p1(calcGLX(cursorPos.x), calcGLY(cursorPos.y + gh)),
		    p2(calcGLX(cursorPos.x + gw), calcGLY(cursorPos.y)),
		    p3(calcGLX(cursorPos.x + gw), calcGLY(cursorPos.y + gh));

		pos.push_back(p2);
		pos.push_back(p0);
		pos.push_back(p1);
		pos.push_back(p3);
		pos.push_back(p2);
		pos.push_back(p1);

		UVs.push_back(uvs[2]);
		UVs.push_back(uvs[0]);
		UVs.push_back(uvs[1]);
		UVs.push_back(uvs[3]);
		UVs.push_back(uvs[2]);
		UVs.push_back(uvs[1]);

		numVerts += 6;
		cursorPos.x += gw;
	}

	vao.bind();
	vao.storeAttribData(0, 2, (float*) pos.data(), pos.size() * sizeof(glm::vec2));
	vao.storeAttribData(1, 2, (float*) UVs.data(), UVs.size() * sizeof(glm::vec2));
	vao.unbind();
}

void Text::render(Shader* s, int x, int y, glm::vec4 color) {
	vao.bind();

	std::vector<ShaderUniform> unis;
	unis.emplace_back("offset", glm::vec2(calcGLX(x) + 1, calcGLY(y) - 1));
	unis.emplace_back("color", color);
	unis.emplace_back("tex", font->getTexture());

	s->setUniforms(unis);
	s->use();
	vao.draw(numVerts);
	s->unuse();

	vao.unbind();
}

} // namespace SWAN
