#ifndef CHECK_COLLISION_HPP
#define CHECK_COLLISION_HPP

#include <glm/glm.hpp>

#include "AABB.hpp"
#include "BoundingSphere.hpp"
#include "Collision.hpp"

Collision CheckCollision(BoundingSphere sphere, glm::vec3 point);
Collision CheckCollision(BoundingSphere a, BoundingSphere b);

Collision CheckCollision(AABB box, glm::vec3 point);
Collision CheckCollision(AABB box, LineSegment ls);
Collision CheckCollision(AABB a, AABB b);

//TODO: CHECK IF THIS WORKS PROPERLY
Collision CheckCollision(BoundingSphere sphere, AABB box);

inline Collision CheckCollision(glm::vec3 point, BoundingSphere sphere){ return CheckCollision(sphere, point); }
inline Collision CheckCollision(glm::vec3 point, AABB box){ return CheckCollision(box, point); }
inline Collision CheckCollision(LineSegment ls, AABB box){ return CheckCollision(box, ls); }
inline Collision CheckCollision(AABB box, BoundingSphere sphere){ return CheckCollision(sphere, box); }

#endif
