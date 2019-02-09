#ifndef PHYSICS_HPP
#define PHYSICS_HPP

// ----- Collision detection / Physics ----- //
#include "SWAN/Core/Defs.hpp"
#include "SWAN/Physics/Basic.hpp"
#include <chrono>

using FloatSeconds = std::chrono::duration<float, std::chrono::seconds::rep>;

enum class ColliderType {
	Plane,
	AABB,
};

struct Collider {
	Collider() {}
	Collider(SWAN::Plane plane)
	    : Type(ColliderType::Plane),
	      Plane(plane) {}
	Collider(SWAN::AABB aabb)
	    : Type(ColliderType::AABB),
	      AABB(aabb) {}

	ColliderType Type;

	SWAN::Plane Plane;
	SWAN::AABB AABB;
};

SWAN::Intersection FindIntersection(const Collider& a, const Collider& b)
{
	switch(a.Type) {
		case ColliderType::Plane:
			switch(b.Type) {
				case ColliderType::Plane: return SWAN::FindIntersection(a.Plane, b.Plane);
				case ColliderType::AABB: return SWAN::FindIntersection(a.Plane, b.AABB);
			}
			break;
		case ColliderType::AABB:
			switch(b.Type) {
				case ColliderType::Plane: return SWAN::FindIntersection(a.AABB, b.Plane);
				case ColliderType::AABB: return SWAN::FindIntersection(a.AABB, b.AABB);
			}
			break;
	}
}

struct PhysicsObject {
	PhysicsObject(SWAN::AABB aabb)
	{
		Collider.AABB = aabb;
		Collider.Type = ColliderType::AABB;
	}

	PhysicsObject(SWAN::Plane plane)
	{
		Collider.Plane = plane;
		Collider.Type = ColliderType::Plane;
	}

	struct Collider Collider;
	SWAN::Transform Transform;
	SWAN::vec3 Velocity = { 0, 0, 0 };
	double Weight = 1;

	inline bool IsImmovable() const { return Collider.Type == ColliderType::Plane; }
};

SWAN::Intersection FindIntersection(const PhysicsObject& a, const PhysicsObject& b)
{
	Collider aCol, bCol;
	if(a.Collider.Type == ColliderType::AABB)
		aCol = Collider(a.Collider.AABB.ApplyTransform(a.Transform));
	else
		aCol = a.Collider;

	if(b.Collider.Type == ColliderType::AABB)
		bCol = Collider(b.Collider.AABB.ApplyTransform(b.Transform));
	else
		bCol = b.Collider;

	return FindIntersection(aCol, bCol);
}

struct PhysicsWorld {
	void AddPhysicsObject(PhysicsObject object)
	{
		PhysicsObjects.push_back(object);
	}

	SWAN::Vector<PhysicsObject> PhysicsObjects;

	void Update(FloatSeconds dt)
	{
		for(auto it = PhysicsObjects.begin(); it != PhysicsObjects.end(); it++) {
			PhysicsObject& po = *it;

			if(!po.IsImmovable())
				po.Velocity += SWAN::vec3(0, -9.8, 0) * po.Weight * dt.count();

			for(auto jt = it + 1; jt != PhysicsObjects.end(); jt++) {
				PhysicsObject& other = *jt;
				SWAN::Intersection intersection = FindIntersection(po, other);

				if(intersection) {
					if(po.IsImmovable() && !other.IsImmovable()) {
						double dot = SWAN::Dot(other.Velocity, po.Collider.Plane.normal);
						other.Velocity -= po.Collider.Plane.normal * dot;
					} else if(!po.IsImmovable() && other.IsImmovable()) {
						double dot = SWAN::Dot(po.Velocity, other.Collider.Plane.normal);
						po.Velocity -= other.Collider.Plane.normal * dot;
					} else if(!po.IsImmovable() && !other.IsImmovable()) {
						SWAN::vec3 vel1 = po.Velocity;
						SWAN::vec3 vel2 = other.Velocity;

						po.Velocity = other.Velocity = (vel1 * po.Weight + vel2 * other.Weight) / 2.0;
						// SWAN::Log("Cubes collide");
					}
				}
				// SWAN::Log(SWAN::Format("Final velocity: {}", other.Velocity * dt.count()));
			}

			po.Transform.pos += po.Velocity;
		}
	}
};

#endif
