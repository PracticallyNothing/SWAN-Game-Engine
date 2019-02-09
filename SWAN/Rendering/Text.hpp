#ifndef SWAN_TEXT_HPP
#define SWAN_TEXT_HPP

#include <string>

#include "BitmapFont.hpp"
#include "GUI/Rect2D.hpp"
#include "Shader.hpp"

#include "OpenGL/VAO.hpp"

namespace SWAN
{
	struct Text {
		Text() : text(""), font(nullptr) {}
		Text(std::string text, const BitmapFont* font) : text(text), font(font)
		{
			updateVAO();
		}

		void render(int x, int y, vec4 color = { 1, 1, 1, 0 }) const;
		void render(Shader* s, int x, int y, vec4 color = { 1, 1, 1, 0 }) const;

		void RenderClipped(int x, int y, Rect2D clip, vec4 color = { 1, 1, 1, 0 }) const;

		void updateVAO();
		void updateVAOClipped(Rect2D clip);

		Text& operator=(const std::string& txt)
		{
			if(text != txt) {
				text = txt;
				updateVAO();
			}
			return *this;
		}
		Text& operator=(const char* const txt)
		{
			if(text != txt) {
				text = txt;
				updateVAO();
			}
			return *this;
		}

		operator std::string&() { return text; }
		operator std::string() const { return text; }
		operator const std::string&() const { return text; }
		operator const char* const() const { return text.c_str(); }

		GL::VAO vao;
		std::string text;
		const BitmapFont* font;
		int numVerts = 0;
	};
} // namespace SWAN

#endif
