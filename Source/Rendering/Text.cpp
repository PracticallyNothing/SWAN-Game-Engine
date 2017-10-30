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

void Text::initCharMesh() { charMesh = genRect2D_ptr(); }

void Text::set(TextConfig tc, bool append) {
	if(!tc.font) {
		std::cout << "ERROR(Text::set()): No font was set\n";
		return;
	}
	this->font = tc.font;

	if(!append) {
		chars.clear();
		lastX = 0;
		lastY = 0;
	}

	int currLine = 0;
	int ii       = 0;

	int x = 0;
	int y = 0;

	for(unsigned i = 0; i < tc.text.length(); i++) {
		char c = tc.text[i];

		x = lastX + font->getGlyphWidth(c) * (i - ii);
		y = lastY + font->getGlyphHeight() * currLine;

		Character res;

		if(c == ' ') {
			res.isSpace = true;
			chars.push_back(res);
			continue;
		} else if(c == '\n') {
			lastX = 0;
			currLine++;
			ii = i + 1;
			continue;
		}

		SWAN_DEBUG_DO(res.c = c);
		res.tex       = font->getGlyphTexture(c, tc.bold, tc.italics);
		res.mesh      = charMesh;
		res.color     = tc.color;
		res.transform = Transform(
		    glm::vec3(
		        SWAN::Util::PixelToGLCoord(
		            Display::GetWidth(),
		            x + font->getGlyphWidth(c) / 2.0),
		        SWAN::Util::PixelToGLCoord(
		            Display::GetHeight(),
		            Display::GetHeight() - y -
		                font->getGlyphHeight() / 2.0),
		        0),
		    glm::vec3(0, 0, 0),
		    glm::vec3((double) font->getGlyphWidth(c) / Display::GetWidth(),
		              (double) font->getGlyphHeight() / Display::GetHeight(),
		              0));

		chars.push_back(res);
	}

	x = lastX + font->getGlyphWidth(' ') * (tc.text.length() - ii);
	y = lastY + font->getGlyphHeight() * currLine;

	if(chars.size()) {
		lastX = x;
		lastY = y;
	}
}

void Text::render(Shader* shad) const {
	for(unsigned i = 0; i < chars.size(); i++) {
		if(chars[i].isSpace)
			continue;

		std::vector<ShaderUniform> uniforms = {
			{ "model", chars[i].transform.getModel() },
			{ "color", chars[i].color }
		};

		shad->setUniforms(uniforms);
		chars[i].tex->bind();
		shad->renderMesh(*chars[i].mesh);
	}
}

void RenderText(int x, int y, std::string text, Shader* s, const BitmapFont* f) {
	assert(s);

	assert(s->hasUniform("transform"));
	assert(s->hasUniform("UVtransform"));

	std::unique_ptr<Mesh> rect(genRect2D_ptr());

	int letter = 0,
	    line   = 0;

	for(int i = 0; i < text.length(); i++) {
		char c = text[i];

		if(c == ' ') {
			letter++;
			continue;
		} else if(c == '\t') {
			letter += f->tabWidth;
		} else if(c == '\n') {
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
		unis.emplace_back("tex", f->getTexture());

		s->setUniforms(unis);
		s->renderMesh(*rect.get());

		letter++;
	}
}
} // namespace SWAN
