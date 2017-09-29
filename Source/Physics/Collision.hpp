#ifndef COLLISION_HPP
#define COLLISION_HPP

/// Struct containing information about a collision
/// Note: Some collisions may not record a distance if the [happened] variable is false.
struct Collision {
	bool happened = false;
	float distance;
};

#endif
