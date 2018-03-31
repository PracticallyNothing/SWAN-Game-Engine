#ifndef SWAN_TEXT_HPP
#define SWAN_TEXT_HPP

#include <string>

#include "BitmapFont.hpp"
#include "OpenGL/VAO.hpp"
#include "Shader.hpp"

namespace SWAN {
struct Text {
	Text(std::string text, const BitmapFont* font)
	    : text(text), font(font) {
		updateVAO();
	}

	void render(Shader* s, int x, int y, vec4 color = { 1, 1, 1, 0 });
	void updateVAO();

	GL::VAO vao;
	std::string text;
	const BitmapFont* font;
	int numVerts = 0;
};
} // namespace SWAN

#endif
