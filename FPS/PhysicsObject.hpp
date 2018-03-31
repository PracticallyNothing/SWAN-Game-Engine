#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <glm/fwd.hpp> // For glm::vec3
#include "../../Source/Physics/AABB.hpp" // For SWAN::AABB

struct PhysObj {
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 vel = glm::vec3(0, 0, 0);
	double forceEffect = 1.0;
	double weight;
	SWAN::AABB colShape;
};

inline bool IsValidPhysObj(const PhysObj& po) { return po.weight >= 0.0 && forceEffect >= 0.0; }

void ApplyForce(PhysObj& po, glm::vec3 force) {
	po.vel += force;
}



#endif
