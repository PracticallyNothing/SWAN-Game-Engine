#ifndef SWAN_DEBUG_RENDER_HPP
#define SWAN_DEBUG_RENDER_HPP

#include <vector>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

#include "Maths/Vector.hpp"
#include "Physics/Transform.hpp"

namespace SWAN {
    struct Material {
	Material() {}
	Material(Texture* diffuse, vec3 spec, float shine)
	    : diffuseTex(diffuse), specularColor(spec), shininess(shine) {}

	const Texture* diffuseTex = nullptr;
	vec3 specularColor;
	float shininess;
    };
    struct Actor {
	Actor() {}
	Actor(const Mesh* m, Material mat, Transform t) : mesh(m), material(mat), transform(t) {}

	const Mesh* mesh;
	Material material;
	Transform transform;
    };

    struct Cube {
	Cube() {}
	Cube(Transform t, vec4 color = {1.0, 1.0, 1.0, 1.0})
	    : transform(t), color(color) {}

	vec4 color = {1.0, 1.0, 1.0, 1.0};
	Transform transform;
    };
    struct DrawnSphere {
	vec4 color = {1.0, 1.0, 1.0, 1.0};
	vec3 center;
	double radius = 1.0;
	unsigned int detail = 16;
    };
    struct DrawnLine {
	DrawnLine() {}
	DrawnLine(vec3 start, vec3 end, vec4 color = {1.0, 1.0, 1.0, 1.0})
	    : start(start), end(end), color(color) {}

	vec4 color = {1.0, 1.0, 1.0, 1.0};
	fvec3 start, end;
	/// @warning This currently does nothing.
	double thickness = 1.0;
	bool endpoints = false;
    };
    struct Arrow : DrawnLine {};
    struct DrawnTriangle {
	/// Constructs a default triangle
	DrawnTriangle() {}
	/// Constructs a triangle with the given coordinates, color and twosidedness.
	DrawnTriangle(vec3 a, vec3 b, vec3 c, vec4 color = {1.0, 1.0, 1.0, 1.0}, bool twoSided = true)
	    : color(color), twoSided(twoSided){
	    points[0] = a;
	    points[1] = b;
	    points[2] = c;
	}

	bool twoSided = false;

	vec4 color = {1.0, 1.0, 1.0, 1.0};
	fvec3 points[3];
	/// @warning This currently does nothing.
	vec3 normal;
    };

    struct RenderTarget {};
    const RenderTarget DefaultFramebuffer;

    extern void Render(
	const Camera& cam,
	Cube c,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(
	const Camera& cam,
	DrawnSphere s,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(
	const Camera& cam,
	Arrow a,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(
	const Camera& cam,
	Actor a,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(
	const Camera& cam,
	DrawnLine l,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(
	const Camera& cam,
	DrawnTriangle t,
	RenderTarget rt = DefaultFramebuffer,
	bool wireframe = false
    );

    extern void Render(const Camera& cam,
		       const std::vector<DrawnLine>& lines,
		       RenderTarget rt = DefaultFramebuffer,
		       bool wireframe = false);
    extern void Render(const Camera& cam,
		       const std::vector<DrawnTriangle>& triangles,
		       RenderTarget rt = DefaultFramebuffer,
		       bool wireframe = false);
}

#endif
