#include "DebugRender.hpp"
#include "../OpenGL/OnGLInit.hpp"
#include "../Utility/CxArray.hpp"

static const char* const unlitFrag = R"glsl(
#version 130

out vec4 fCol;

uniform vec4 color;

void main() { 
	fCol = color; 
}
)glsl";

static const char* const unlitVert = R"glsl(
#version 130

in vec3 pos;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

void main() {
    gl_Position = perspective * view * transform * vec4(pos, 1);
    gl_PointSize = 20;
}
)glsl";

namespace SWAN
{
	static Shader basicShad;
	static GL::VAO cubeVAO;
	static GL::VAO lineVAO;
	static GL::VAO linesVAO;
	static GL::VAO triVAO;
	static GL::VAO trisVAO;

	static OnGLInit _ = {
		[] {
		    basicShad.compileShadersFromSrc(unlitVert, unlitFrag);
		    basicShad.addAttrib("pos");
		    basicShad.linkShaders();

		    basicShad.use();
		    basicShad.addUniform("color");
		    basicShad.addUniform("perspective");
		    basicShad.addUniform("view");
		    basicShad.addUniform("transform");
		    // basicShad.unuse();
		},

		[] {
		    const Util::CxArray<fvec3, 8> pos({
		        /* 4 */ fvec3(-1, -1, -1),
		        /* 5 */ fvec3(1, 1, -1),
		        /* 6 */ fvec3(1, -1, -1),
		        /* 7 */ fvec3(-1, 1, -1),

		        /* 0 */ fvec3(-1, -1, 1),
		        /* 1 */ fvec3(1, 1, 1),
		        /* 2 */ fvec3(1, -1, 1),
		        /* 3 */ fvec3(-1, 1, 1),
		    });

		    /*
	     *     3   1
	     *     o---o
	     *   7/  5/|
	     *   o---o |
	     *   |  /| |
	     *  0| / | o 2
	     *   |/  |/
	     *   o---o 
	     *   4   6   
	     *
	     */

		    const Util::CxArray<unsigned, 36> inds(
		        { 0, 2, 1, // 0, 1, 2
		          3, 0, 1, // 3, 4, 5

		          7, 4, 3, // 6, 7, 8
		          4, 0, 3, // 9, 10, 11

		          1, 2, 5, // 12, 13, 14
		          2, 6, 5, // 15, 16, 17

		          7, 5, 6, // 18, 19, 20
		          7, 6, 4, // 21, 22, 23

		          3, 1, 7, // 24, 25, 26
		          1, 5, 7, // 27, 28, 29

		          0, 6, 2,
		          0, 4, 6 });

		    cubeVAO.bind();
		    cubeVAO.storeAttribData(0, 3, (float*) pos.data(), pos.size() * sizeof(vec3), GL_STATIC_DRAW);
		    cubeVAO.storeIndices(inds.data(), inds.size() * sizeof(unsigned), GL_STATIC_DRAW);
		    cubeVAO.unbind();
		},
	};

	void Render(const Camera& cam, Cube c, RenderTarget rt, bool wireframe)
	{
		basicShad.use();
		basicShad.SetVec4("color", c.color);
		basicShad.SetMat4("transform", c.transform.getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());
		cubeVAO.draw(36, wireframe ? GL_LINE_LOOP : GL_TRIANGLES);
		//basicShad.unuse();
	}
	void Render(const Camera& cam, DrawnSphere s, RenderTarget rt, bool wireframe) {}

	void Render(const Camera& cam, DrawnLine l, RenderTarget rt, bool wireframe)
	{
		basicShad.use();
		basicShad.SetVec4("color", l.color);
		basicShad.SetMat4("transform", Transform().getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());
		fvec3 p[2] = { l.start, l.end };
		lineVAO.storeAttribData(0, 3, (float*) p, 2 * sizeof(vec3), GL_DYNAMIC_DRAW);
		lineVAO.draw(2, GL_LINES);
		// basicShad.unuse();
	}

	void Render(const Camera& cam, const std::vector<DrawnLine>& lines, RenderTarget rt, bool wireframe)
	{
		if(lines.empty())
			return;

		std::vector<fvec3> p;

		for(auto l : lines) {
			p.push_back(l.start);
			p.push_back(l.end);
		}

		basicShad.use();
		basicShad.SetVec4("color", lines[0].color);
		basicShad.SetMat4("transform", Transform().getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());
		linesVAO.storeAttribData(0, 3, (float*) p.data(), p.size() * sizeof(vec3), GL_DYNAMIC_DRAW);
		linesVAO.draw(p.size(), GL_LINES);
	}

	void Render(const Camera& cam, Arrow a, RenderTarget rt, bool wireframe)
	{
		basicShad.use();
		basicShad.SetVec4("color", a.color);
		basicShad.SetMat4("transform", Transform().getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());

		fvec3 p[2] = { a.start, a.end };
		lineVAO.storeAttribData(0, 3, (float*) p, 2 * sizeof(vec3), GL_DYNAMIC_DRAW);
		lineVAO.draw(2, GL_LINE_LOOP);
		basicShad.unuse();
	}
	void Render(const Camera& cam, Actor a, RenderTarget rt, bool wireframe) {}

	void Render(const Camera& cam, DrawnTriangle t, RenderTarget rt, bool wireframe)
	{
		if(t.twoSided) {
			std::array<fvec3, 6> v = { t.points[0], t.points[1], t.points[2],
				                       t.points[2], t.points[1], t.points[0] };
			triVAO.storeAttribData(0, 3, (float*) v.data(), 6 * sizeof(vec3), GL_DYNAMIC_DRAW);
		} else
			triVAO.storeAttribData(0, 3, (float*) t.points, 3 * sizeof(vec3), GL_DYNAMIC_DRAW);

		basicShad.use();
		basicShad.SetVec4("color", t.color);
		basicShad.SetMat4("transform", Transform().getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());
		triVAO.draw(t.twoSided ? 6 : 3, wireframe ? GL_LINE_STRIP : GL_TRIANGLES);
		basicShad.unuse();
	}

	void Render(const Camera& cam, const std::vector<DrawnTriangle>& triangles, RenderTarget rt, bool wireframe)
	{
		if(triangles.empty())
			return;

		std::vector<fvec3> v;
		for(auto t : triangles) {
			v.push_back(t.points[0]);
			v.push_back(t.points[1]);
			v.push_back(t.points[2]);

			if(t.twoSided) {
				v.push_back(t.points[2]);
				v.push_back(t.points[1]);
				v.push_back(t.points[0]);
			}
		}
		trisVAO.storeAttribData(0, 3, (float*) v.data(), v.size() * sizeof(vec3), GL_DYNAMIC_DRAW);

		basicShad.use();
		basicShad.SetVec4("color", triangles[0].color);
		basicShad.SetMat4("transform", Transform().getModel());
		basicShad.SetMat4("perspective", cam.getPerspective());
		basicShad.SetMat4("view", cam.getView());
		trisVAO.draw(v.size(), wireframe ? GL_LINE_STRIP : GL_TRIANGLES);
	}
} // namespace SWAN
