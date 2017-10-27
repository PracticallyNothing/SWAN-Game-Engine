#ifndef SWAN_COLLISION_HPP
#define SWAN_COLLISION_HPP

namespace SWAN {
/// Struct containing information about a collision
/// Note: Some collisions may not record a distance if the [happened] variable is false.
struct Collision {
	bool  happened = false;
	float distance;
};
}

#endif
