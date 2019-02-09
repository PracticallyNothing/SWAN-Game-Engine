#include "SpriteSheet.hpp"

#include "Shader.hpp"

#include "Core/Display.hpp"

#include "OpenGL/OnGLInit.hpp"
#include "OpenGL/VAO.hpp"

#include "Utility/CxArray.hpp"
#include "Utility/Math.hpp"

static SWAN::Shader spriteShader;

static const char* spriteVert = R"glsl(
#version 130

in vec2 pos;
in vec2 UV;
out vec2 _UV;

void main() {
    gl_Position.xy = pos;
    _UV = UV;
}
)glsl";

static const char* spriteFrag = R"glsl(
#version 130

in vec2 _UV;
out vec4 fCol;

uniform sampler2D tex;

uniform float overrideColorInfluence = 0;
uniform vec3 overrideColor;
uniform float overrideAlphaInfluence = 0;
uniform float overrideAlpha;

void main() {
    vec4 c = texture2D(tex, _UV);
    fCol.rgb = mix(c.rgb, overrideColor, overrideColorInfluence);
    fCol.a = mix(c.a, overrideAlpha, overrideAlphaInfluence);
}
)glsl";

static SWAN::OnGLInit _ = {
	[] {
	    spriteShader.compileShadersFromSrc(spriteVert, spriteFrag);
	    spriteShader.addAttrib("pos");
	    spriteShader.addAttrib("UV");
	    spriteShader.linkShaders();

	    spriteShader.use();

	    spriteShader.addUniform("overrideColorInfluence");
	    spriteShader.addUniform("overrideColor");
	    spriteShader.addUniform("overrideAlphaInfluence");
	    spriteShader.addUniform("overrideAlpha");

	    spriteShader.unuse();
	}
};

namespace SWAN
{

	GL::VAO spriteVAO;
	void Sprite::render(ivec2 _pos, ivec2 dim) const
	{
		using Util::CxArray;
		using Util::PixelToGLCoord;

		// Screen width and height
		unsigned
		    scrW
		    = Display::GetWidth(),
		    scrH = Display::GetHeight();

		int x = _pos.x,
		    y = _pos.y;

		int w = dim.x,
		    h = dim.y;

		// Set these to the actual size of the sprites
		// if they're -1.
		w = (w < 0 ? dimensions.x : w);
		h = (h < 0 ? dimensions.y : h);

		// If it's pointless to draw this sprite, return.
		if(w == 0 || h == 0 || x > scrW || y > scrH || x + w < 0 || y + h < 0)
			return;

		CxArray<fvec2, 4> pos{
			/* 1 */ fvec2(PixelToGLCoord(scrW, x), -PixelToGLCoord(scrH, y - h)),
			/* 3 */ fvec2(PixelToGLCoord(scrW, x + w), -PixelToGLCoord(scrH, y)),
			/* 2 */ fvec2(PixelToGLCoord(scrW, x), -PixelToGLCoord(scrH, y)),
			/* 4 */ fvec2(PixelToGLCoord(scrW, x + w), -PixelToGLCoord(scrH, y - h)),
		};

		// x,     y - h >> 1
		// x,     y     >> 2
		// x + w, y     >> 3
		// x + w, y - h >> 4

		/*
		 * 1     3
		 *  o---o
		 *  |  /|
		 *  | / |
		 *  |/  |
		 *  o---o
		 * 2     4
		 */

		CxArray<fvec2, 4> UV{
			uvMin,
			fvec2(uvMin.x, uvMax.y),
			uvMax,
			fvec2(uvMax.x, uvMin.y)
		};

		spriteVAO.bind();
		spriteVAO.storeAttribData(0, 2, (float*) pos.data(), sizeof(fvec2) * 4);
		spriteVAO.storeAttribData(1, 2, (float*) UV.data(), sizeof(fvec2) * 4);

		spriteShader.use();
		source->bind();
		spriteVAO.draw(4, GL_TRIANGLE_FAN);
		spriteShader.unuse();
	}

	void Sprite::renderOverride(ivec2 _pos, ivec2 dim, vec3 color, double alpha) const
	{
		using Util::CxArray;
		using Util::PixelToGLCoord;

		// Screen width and height
		unsigned
		    scrW
		    = Display::GetWidth(),
		    scrH = Display::GetHeight();

		int x = _pos.x,
		    y = _pos.y;

		int w = dim.x,
		    h = dim.y;

		// Set these to the actual size of the sprites
		// if they're -1.
		w = (w < 0 ? dimensions.x : w);
		h = (h < 0 ? dimensions.y : h);

		// If it's pointless to draw this sprite, return.
		if(w == 0 || h == 0 || x > scrW || y > scrH || x + w < 0 || y + h < 0)
			return;

		CxArray<fvec2, 4> pos{
			/* 1 */ fvec2(PixelToGLCoord(scrW, x), -PixelToGLCoord(scrH, y - h)),
			/* 3 */ fvec2(PixelToGLCoord(scrW, x + w), -PixelToGLCoord(scrH, y)),
			/* 2 */ fvec2(PixelToGLCoord(scrW, x), -PixelToGLCoord(scrH, y)),
			/* 4 */ fvec2(PixelToGLCoord(scrW, x + w), -PixelToGLCoord(scrH, y - h)),
		};

		// x,     y - h >> 1
		// x,     y     >> 2
		// x + w, y     >> 3
		// x + w, y - h >> 4

		/*
		 * 1     3
		 *  o---o
		 *  |  /|
		 *  | / |
		 *  |/  |
		 *  o---o
		 * 2     4
		 */

		CxArray<fvec2, 4> UV{
			uvMin,
			fvec2(uvMin.x, uvMax.y),
			uvMax,
			fvec2(uvMax.x, uvMin.y)
		};

		spriteVAO.bind();
		spriteVAO.storeAttribData(0, 2, (float*) pos.data(), sizeof(fvec2) * 4);
		spriteVAO.storeAttribData(1, 2, (float*) UV.data(), sizeof(fvec2) * 4);

		spriteShader.use();
		spriteShader.SetReal("overrideColorInfluence", 1.0);
		spriteShader.SetVec3("overrideColor", color);
		spriteShader.SetReal("overrideAlphaInfluence", alpha < 0 ? 0 : 1);
		spriteShader.SetReal("overrideAlpha", alpha);
		source->bind();
		spriteVAO.draw(4, GL_TRIANGLE_FAN);
		spriteShader.unuse();
	}
} // namespace SWAN
