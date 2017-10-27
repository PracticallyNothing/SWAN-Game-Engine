#ifndef SWAN_AABB_HPP
#define SWAN_AABB_HPP

#include "Rendering/Camera.hpp"
#include "Transform.hpp"

#include <glm/glm.hpp>

namespace SWAN {
struct AABB {
	glm::vec3 min, max;
};

/// Finite line
struct LineSegment {
	glm::vec3 start, end;
};

/// Returns an AABB adjusted to fit the transform
AABB ApplyTransform(AABB box, Transform t);

/// Renders an AABB onscreen using the "Cube" mesh
void Render(AABB aabb, const Camera* cam, bool colliding = false);
}

#endif
