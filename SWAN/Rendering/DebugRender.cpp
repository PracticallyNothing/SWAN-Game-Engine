#include "DebugRender.hpp"
#include "../Utility/CxArray.hpp"
#include "../OpenGL/OnGLInit.hpp"

namespace SWAN {
    const char* const unlitFrag =
	"#version 130"                   "\n"
	""
	"out vec4 fCol;"                 "\n"
	""
	"uniform vec4 color;"            "\n"
	""
	"void main() { fCol = color; }";

    const char* const unlitVert =
	"#version 130"               "\n"
	""
	"in vec3 pos;"               "\n"
	""
	"uniform mat4 perspective;"  "\n"
	"uniform mat4 view;"         "\n"
	"uniform mat4 transform;"    "\n"
	""
	"void main() {"                                                      "\n"
	"    gl_Position = perspective * view * transform * vec4(pos, 1);"   "\n"
	"    gl_PointSize = 20;"                                             "\n"
	"}";

    Shader basicShad;

    OnGLInit _ = {
	[] {
	    basicShad.compileShadersFromSrc(unlitVert, unlitFrag);
	    basicShad.addAttrib("pos");
	    basicShad.linkShaders();

	    basicShad.use();
	    basicShad.addUniform("perspective");
	    basicShad.addUniform("view");
	    basicShad.addUniform("transform");
	    basicShad.unuse();
	}
    };


    GL::VAO cubeVAO;
    void Render(const Camera& cam, Cube c, RenderTarget rt, bool wireframe) {
	const Util::CxArray<fvec3, 8> pos
	    ({
		/* 4 */ fvec3(-1, -1, -1),
		/* 5 */ fvec3( 1,  1, -1),
		/* 6 */ fvec3( 1, -1, -1),
		/* 7 */ fvec3(-1,  1, -1),

		/* 0 */ fvec3(-1, -1,  1),
		/* 1 */ fvec3( 1,  1,  1),
		/* 2 */ fvec3( 1, -1,  1),
		/* 3 */ fvec3(-1,  1,  1),
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

	const Util::CxArray<unsigned, 36> inds (
	    {0, 2, 1,
	     3, 0, 1,

	     7, 4, 3,
	     4, 0, 3,

	     1, 2, 5,
	     2, 6, 5,

	     4, 5, 6,
	     7, 5, 4,

	     3, 1, 5,
	     3, 5, 7,

	     0, 6, 2,
	     0, 4, 6}); 

	cubeVAO.storeAttribData(0, 3, (float*) pos.data(), pos.size() * sizeof(vec3), GL_STATIC_DRAW);
	cubeVAO.storeIndices(inds.data(), inds.size() * sizeof(unsigned), GL_STATIC_DRAW);

	basicShad.setUniforms({
		{"color", c.color},
		{"transform", c.transform},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });
	basicShad.use();
	//vao.draw(inds.size(), wireframe ? GL_LINE_LOOP : GL_TRIANGLES);
	cubeVAO.draw(inds.size(), wireframe ? GL_LINE_LOOP : GL_TRIANGLES);
	basicShad.unuse();
    }
    void Render(const Camera& cam, DrawnSphere s, RenderTarget rt, bool wireframe) {}

    GL::VAO lineVAO;
    void Render(const Camera& cam, DrawnLine l, RenderTarget rt, bool wireframe) {
	basicShad.setUniforms({
		{"color", l.color},
		{"transform", Transform()},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });

	basicShad.use();
	fvec3 p[2] = { l.start, l.end };
	lineVAO.storeAttribData(0, 3, (float*) p, 2 * sizeof(vec3), GL_DYNAMIC_DRAW);
	lineVAO.draw(2, GL_LINES);
	basicShad.unuse();
    }

    GL::VAO linesVAO;
    void Render(const Camera& cam, const std::vector<DrawnLine>& lines, RenderTarget rt, bool wireframe) {
	if(lines.empty())
	    return;

	basicShad.setUniforms({
		{"color", lines[0].color},
		{"transform", Transform()},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });

	basicShad.use();
	std::vector<fvec3> p;

	for(auto l : lines) {
	    p.push_back(l.start);
	    p.push_back(l.end);
	}
	linesVAO.storeAttribData(0, 3, (float*) p.data(), p.size() * sizeof(vec3), GL_DYNAMIC_DRAW);
	linesVAO.draw(p.size(), GL_LINES);
	basicShad.unuse();
    }
    void Render(const Camera& cam, Arrow a, RenderTarget rt, bool wireframe) {
	basicShad.setUniforms({
		{"color", a.color},
		{"transform", Transform()},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });

	basicShad.use();
	fvec3 p[2] = { a.start, a.end };
	lineVAO.storeAttribData(0, 3, (float*) p, 2 * sizeof(vec3), GL_DYNAMIC_DRAW);
	lineVAO.draw(2, GL_LINE_LOOP);
	basicShad.unuse();
    }
    void Render(const Camera& cam, Actor a, RenderTarget rt, bool wireframe) {}

    GL::VAO triVAO;
    void Render(const Camera& cam, DrawnTriangle t, RenderTarget rt, bool wireframe) {
	if(t.twoSided) {
	    std::array<fvec3, 6> v = { t.points[0], t.points[1], t.points[2],
				       t.points[2], t.points[1], t.points[0] };
	    triVAO.storeAttribData(0, 3, (float*) v.data() , 6 * sizeof(vec3), GL_DYNAMIC_DRAW);
	}
	else triVAO.storeAttribData(0, 3, (float*) t.points, 3 * sizeof(vec3), GL_DYNAMIC_DRAW);

	basicShad.setUniforms({
		{"color", t.color},
		{"transform", Transform()},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });

	basicShad.use();
	triVAO.draw(t.twoSided ? 6 : 3, wireframe ? GL_LINE_STRIP: GL_TRIANGLES);
	basicShad.unuse();
    }

    GL::VAO trisVAO;
    void Render(const Camera& cam, const std::vector<DrawnTriangle>& triangles, RenderTarget rt, bool wireframe) {
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

	trisVAO.storeAttribData(0, 3, (float*) v.data() , v.size() * sizeof(vec3), GL_DYNAMIC_DRAW);
	basicShad.setUniforms({
		{"color", triangles[0].color},
		{"transform", Transform()},
		{"perspective", cam.getPerspective()},
		{"view", cam.getView()},
	    });
	basicShad.use();
	trisVAO.draw(v.size(), wireframe ? GL_LINE_STRIP: GL_TRIANGLES);
	basicShad.unuse();
    }
}
