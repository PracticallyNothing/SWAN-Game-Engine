#ifndef AABB_HPP
#define AABB_HPP

#include "Transform.hpp"
#include "../Rendering/Camera.hpp"

#include <glm/glm.hpp>

struct AABB {
	glm::vec3 min, max;
//	bool colliding = false;
};

/// Finite line
struct LineSegment { glm::vec3 start, end; };

/// Returns an AABB adjusted to fit the transform
AABB ApplyTransform(AABB box, const Transform& t);

/// Renders an AABB onscreen using the "Cube" mesh
void Render(AABB aabb, const Camera* cam);

#endif
