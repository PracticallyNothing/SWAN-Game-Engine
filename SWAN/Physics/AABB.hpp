#ifndef SWAN_AABB_HPP
#define SWAN_AABB_HPP

#include "Rendering/Camera.hpp"
#include "Transform.hpp"
#include "Maths/Vector.hpp"

namespace SWAN {
struct AABB {
	vec3 min, max;
	vec3 center() { return max - min; }
	double xLen() { return max.x - min.x; }
	double yLen() { return max.y - min.y; }
	double zLen() { return max.z - min.z; }
};

/// Finite line
struct LineSegment {
	vec3 start, end;
};

bool PointIsInsideAABB(AABB, vec3);

/// Returns an AABB adjusted to fit the transform
AABB ApplyTransform(AABB box, Transform t);

/// Renders an AABB onscreen using the "Cube" mesh
void Render(AABB aabb, const Camera* cam, bool colliding = false);
} // namespace SWAN

#endif
