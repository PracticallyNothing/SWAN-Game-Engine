#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "SWAN/Maths/Vector.hpp" // For SWAN::vec3
#include "../../Source/Physics/AABB.hpp" // For SWAN::AABB

using SWAN::vec3;
using SWAN::AABB;

struct PhysObj {
    vec3 pos = glm::vec3(0, 0, 0);
    vec3 vel = glm::vec3(0, 0, 0);
    double forceEffect = 1.0;
    double weight;
    AABB colShape;
};

inline bool IsValidPhysObj(const PhysObj& po) {
    return po.weight >= 0.0 && forceEffect >= 0.0;
}

void ApplyForce(PhysObj& po, vec3 force) {
    po.vel += force;
}



#endif
