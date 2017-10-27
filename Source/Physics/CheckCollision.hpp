#ifndef SWAN_CHECK_COLLISION_HPP
#define SWAN_CHECK_COLLISION_HPP

#include <glm/glm.hpp>

#include "AABB.hpp"
//#include "BoundingSphere.hpp"
#include "Collision.hpp"

namespace SWAN {
struct Ray {
	glm::vec3 start, direction;
};
struct Plane {
	glm::vec3 normal;
	double    offset;
};

enum HalfSpaceRes {
	HS_FRONT,
	HS_BACK,
	HS_ON_PLANE
};

/// Test if a point is below, above or on a plane
HalfSpaceRes
HalfSpaceTest (glm::vec3 point, Plane plane);

/*
	 * Collision CheckCollision(BoundingSphere sphere, glm::vec3 point);
	 * Collision CheckCollision(BoundingSphere sphere, Ray ray);
	 * Collision CheckCollision(BoundingSphere sphere, Plane plane);
	 * Collision CheckCollision(BoundingSphere a, BoundingSphere b);
	 */

Collision
CheckCollision (AABB box, glm::vec3 point);
Collision
CheckCollision (AABB box, Ray ray);
Collision
CheckCollision (AABB box, Plane plane);
Collision
CheckCollision (AABB a, AABB b);

// Collision CheckCollision (BoundingSphere sphere, AABB box);

/*
	 * inline Collision CheckCollision (glm::vec3 point, BoundingSphere sphere) { return CheckCollision (sphere, point); }
	 * inline Collision CheckCollision (Ray ray, BoundingSphere sphere)         { return CheckCollision (sphere, ray);   }
	 * inline Collision CheckCollision (Plane plane, BoundingSphere sphere)     { return CheckCollision (sphere, plane); }
	 */

inline Collision
CheckCollision (glm::vec3 point, AABB box) { return CheckCollision (box, point); }
inline Collision
CheckCollision (Ray ray, AABB box) { return CheckCollision (box, ray); }
inline Collision
CheckCollision (Plane plane, AABB box) { return CheckCollision (box, plane); }

// inline Collision CheckCollision (AABB box, BoundingSphere sphere) { return CheckCollision (sphere, box);   }
}
#endif
