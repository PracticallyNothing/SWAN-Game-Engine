#include "Text.hpp"

#include "Core/Display.hpp" // For Display::GetWidth(), Display::GetHeight()

#include "Utility/Math.hpp" // For SWAN::Util::GLToPixelCoord()
#include "Utility/StreamOps.hpp"

/*
 *  v0      v1
 *   o-----o
 *   |    /|
 *   |   / |
 *   |  /  |
 *   | /   |
 *   |/    |
 *   o-----o
 *  v2      v3
 */
namespace SWAN {
static Mesh* genRect2D_ptr() {
	Vertex v0(glm::vec3(-1, 1, 0), glm::vec2(0, 1),
	          glm::normalize(glm::vec3(-0.5, -0.5, 0.5)));

	Vertex v1(glm::vec3(1, 1, 0), glm::vec2(1, 1),
	          glm::normalize(glm::vec3(0.5, 0.5, 0.5)));

	Vertex v2(glm::vec3(-1, -1, 0), glm::vec2(0, 0),
	          glm::normalize(glm::vec3(0.5, -0.5, 0.5)));

	Vertex v3(glm::vec3(1, -1, 0), glm::vec2(1, 0),
	          glm::normalize(glm::vec3(-0.5, 0.5, 0.5)));

	return new Mesh({ v0, v1, v2, v3 }, { 0, 2, 1, 1, 2, 3 });
}

void RenderText(
    int x, int y,
    std::string text,
    Shader* s,
    const BitmapFont* f,
    SWAN::Color color,
    SWAN::Color bgColor) {
	assert(s);

	assert(s->hasUniform("transform"));
	assert(s->hasUniform("UVtransform"));

	glClear(GL_DEPTH_BUFFER_BIT);

	std::unique_ptr<Mesh> rect(genRect2D_ptr());

	int letter = 0,
	    line   = 0;

	bool newLineVis = false;

	for(int i = 0; i < text.length(); i++) {
		char c = text[i];

		if(bgColor.alpha == 0) {
			if(c == ' ') {
				letter++;
				continue;
			} else if(c == '\t') {
				letter += f->tabWidth;
				continue;
			}
		}

		if(c == '\n') {
			line++;
			letter = 0;
			continue;
		}

		int gw = f->getGlyphWidth(c),
		    gh = f->getGlyphHeight();

		Transform transform;
		transform.scale.x = (float) gw / Display::GetWidth();
		transform.scale.y = (float) gh / Display::GetHeight();

		transform.pos.x = SWAN::Util::PixelToGLCoord(Display::GetWidth(), x + letter * gw + gw / 2);
		transform.pos.y = SWAN::Util::PixelToGLCoord(
		    Display::GetHeight(),
		    Display::GetHeight() - (y + line * gh + gh / 2));

		std::vector<ShaderUniform> unis;
		unis.emplace_back("transform", transform);
		unis.emplace_back("UVtransform", f->getGlyphUVTransform(c));
		unis.emplace_back("color", (glm::vec4) color);
		unis.emplace_back("bgColor", (glm::vec4) bgColor);
		unis.emplace_back("tex", f->getTexture());

		s->setUniforms(unis);
		s->renderMesh(*rect.get());

		letter++;
	}
}
} // namespace SWAN
