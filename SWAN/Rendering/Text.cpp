#include "Text.hpp"

#include "Core/Display.hpp" // For SWAN::Display::GetWidth(), SWAN::Display::GetHeight()
#include "Core/Format.hpp"
#include "Core/Logging.hpp"

#include "Camera.hpp"
#include "OpenGL/OnGLInit.hpp" // For SWAN::OnGLInit()

#include "Maths/Vector.hpp" // For SWAN::vec2, SWAN::vec3

#include "Utility/Debug.hpp"
#include "Utility/FindEx.hpp"     // For SWAN::Util::FindNth()
#include "Utility/Math.hpp"       // For SWAN::Util::Normalize()
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()

#include <algorithm>
#include <iostream>
#include <vector>

using namespace SWAN::Util;

static const char* textVert = R"ddd(
#version 130

in vec2 pos;
in vec2 UV;

out vec2 fUV;

uniform vec2 offset;
uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * vec4(pos + offset, 0, 1);
    fUV = UV;
}
)ddd";

static const char* textFrag = R"ddd(
#version 130

in vec2 fUV;

out vec4 fCol;

uniform vec4 color = vec4(0, 0, 0, 1);
uniform sampler2D tex;

void main(){
    fCol = vec4(color.rgb, texture2D(tex, vec2(fUV.x, fUV.y)).a);
}
)ddd";

static SWAN::Shader textShader;

SWAN::OnGLInit _(
    [] {
	    textShader.compileShadersFromSrc(textVert, textFrag);
	    textShader.addAttrib("pos");
	    textShader.addAttrib("UV");
	    textShader.linkShaders();

	    textShader.use();
	    textShader.addUniform("offset");
	    textShader.addUniform("viewProj");
	    textShader.addUniform("color");
	    textShader.unuse();
    });

namespace SWAN
{
	void Text::updateVAO()
	{
		if(Util::Trim(text).length() < 1)
			return;

		std::vector<fvec2> pos, UVs;

		pos.reserve(text.length() * 6);
		UVs.reserve(text.length() * 6);

		ivec2 cursorPos;

		int verts = 0;

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

			char c = text[i];
			int gw = font->getGlyphWidth(c),
			    gh = font->getGlyphHeight();

			fvec2 p0 = cursorPos,
			      p1 = cursorPos + ivec2(0, gh),
			      p2 = cursorPos + ivec2(gw, 0),
			      p3 = cursorPos + ivec2(gw, gh);

			pos.push_back(p2);
			pos.push_back(p0);
			pos.push_back(p1);
			pos.push_back(p3);
			pos.push_back(p2);
			pos.push_back(p1);

			auto uvs = font->getGlyphUVs(c);
			UVs.push_back(uvs[2]);
			UVs.push_back(uvs[0]);
			UVs.push_back(uvs[1]);
			UVs.push_back(uvs[3]);
			UVs.push_back(uvs[2]);
			UVs.push_back(uvs[1]);

			verts += 6;
			cursorPos.x += gw;
		}

		vao.bind();
		vao.storeAttribData(0, 2, (float*) pos.data(), pos.size() * sizeof(fvec2), GL_DYNAMIC_DRAW);
		vao.storeAttribData(1, 2, (float*) UVs.data(), UVs.size() * sizeof(fvec2), GL_DYNAMIC_DRAW);

		numVerts = verts;
	}

	static constexpr ivec2 Min(ivec2 a, ivec2 b)
	{
		ivec2 res;
		res.x = a.x <= b.x ? a.x : b.x;
		res.y = a.y <= b.x ? a.y : b.y;
		return res;
	}

	static constexpr ivec2 Max(ivec2 a, ivec2 b)
	{
		ivec2 res;
		res.x = a.x <= b.x ? b.x : a.x;
		res.y = a.y <= b.x ? b.y : a.y;
		return res;
	}

	void Text::updateVAOClipped(Rect2D clip)
	{
		if(Util::Trim(text).length() < 1)
			return;

		if(clip.Size.x == 0 || clip.Size.y == 0)
			return;

		std::vector<fvec2> pos, UVs;

		pos.reserve(text.length() * 6);
		UVs.reserve(text.length() * 6);

		if(clip.Position.x < 0) {
			clip.Size.x += clip.Position.x;
			clip.Position.x = 0;
		}
		if(clip.Position.y < 0) {
			clip.Size.y += clip.Position.y;
			clip.Position.y = 0;
		}

		int verts = 0;
		ivec2 cursorPos;

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

			char c = text[i];
			auto uvs = font->getGlyphUVs(c);
			int gw = font->getGlyphWidth(c),
			    gh = font->getGlyphHeight();

			if(cursorPos.x + gw <= clip.Position.x || cursorPos.y + gh <= clip.Position.y || cursorPos.x >= (clip.Position + clip.Size).x || cursorPos.y >= (clip.Position + clip.Size).y) {
				cursorPos.x += gw;
				continue;
			}

			fvec2 p0(Util::Clamp(cursorPos.x, clip.Position.x, (clip.Position + clip.Size).x),
			         Util::Clamp(cursorPos.y, clip.Position.y, (clip.Position + clip.Size).y)),
			    p1(p0.x, Util::Clamp(cursorPos.y + gh, clip.Position.x, (clip.Position + clip.Size).y)),
			    p2(Util::Clamp(cursorPos.x + gw, clip.Position.x, (clip.Position + clip.Size).x), p0.y),
			    p3(p2.x, p1.y);

			pos.push_back(p2);
			pos.push_back(p0);
			pos.push_back(p1);
			pos.push_back(p3);
			pos.push_back(p2);
			pos.push_back(p1);

			/// How much change in UV is one pixel?
			fvec2 uvUnit = uvs[3] - uvs[0];
			uvUnit.x /= gw;
			uvUnit.y /= gh;

			fvec2 origP0(cursorPos),
			    origP1(cursorPos + ivec2(0, gh)),
			    origP2(cursorPos + ivec2(gw, 0)),
			    origP3(cursorPos + ivec2(gw, gh));

			ivec2 diff0 = p0 - origP0;
			uvs[0].x += diff0.x * uvUnit.x;
			uvs[0].y += diff0.y * uvUnit.y;

			ivec2 diff1 = p1 - origP1;
			uvs[1].x += diff1.x * uvUnit.x;
			uvs[1].y += diff1.y * uvUnit.y;

			ivec2 diff2 = p2 - origP2;
			uvs[2].x += diff2.x * uvUnit.x;
			uvs[2].y += diff2.y * uvUnit.y;

			ivec2 diff3 = p3 - origP3;
			uvs[3].x += diff3.x * uvUnit.x;
			uvs[3].y += diff3.y * uvUnit.y;

			UVs.push_back(uvs[2]);
			UVs.push_back(uvs[0]);
			UVs.push_back(uvs[1]);
			UVs.push_back(uvs[3]);
			UVs.push_back(uvs[2]);
			UVs.push_back(uvs[1]);

			verts += 6;
			cursorPos.x += gw;
		}

		vao.bind();
		vao.storeAttribData(0, 2, (float*) pos.data(), pos.size() * sizeof(fvec2), GL_DYNAMIC_DRAW);
		vao.storeAttribData(1, 2, (float*) UVs.data(), UVs.size() * sizeof(fvec2), GL_DYNAMIC_DRAW);

		numVerts = verts;
	}

	void Text::render(int x, int y, vec4 color) const { render(&textShader, x, y, color); }
	void Text::render(Shader* s, int x, int y, vec4 color) const
	{
		if(Util::Trim(text).length() < 1)
			return;

		Camera c = Camera(OrthographicT());

		s->use();
		s->SetVec2("offset", vec2(x, y));
		s->SetMat4("viewProj", c.getPerspective() * c.getView());
		s->SetVec4("color", color);
		font->getTexture()->bind();
		vao.draw(numVerts);
	}

	void Text::RenderClipped(int x, int y, Rect2D clip, vec4 color) const
	{
		if(Util::Trim(text).length() < 1)
			return;

		if(x + font->getTextWidth(text) <= clip.Position.x || y + font->getTextHeight(text) <= clip.Position.y || x >= (clip.Position + (ivec2) clip.Size).x || y >= (clip.Position + (ivec2) clip.Size).y) {
			return;
		}

		Camera c = Camera(OrthographicT());

		clip.Position -= ivec2(x, y);

		// "No, really, I'm not const!"
		((Text*) this)->updateVAOClipped(clip);

		textShader.use();
		textShader.SetVec2("offset", vec2(x, y));
		textShader.SetMat4("viewProj", c.getPerspective() * c.getView());
		textShader.SetVec4("color", color);
		font->getTexture()->bind();
		vao.draw(numVerts);
	}
} // namespace SWAN
