#ifndef COL_WRAPPER_HPP
#define COL_WRAPPER_HPP

#include "AABB.hpp"
#include "BoundingSphere.hpp"
#include "CheckCollision.hpp"

struct ColWrapper {
	enum Type {
		COL_AABB,
		COL_SPHERE
	} type;

	union {
		AABB aabb;
		BoundingSphere sphere;
	};

	Collision checkCollision(const ColWrapper& other){
		if(type == other.type){
			switch(type){
				case COL_AABB:
					return CheckCollision(aabb, other.aabb);
				case COL_SPHERE:
					return CheckCollision(sphere, other.sphere);
			}
		} else {
			switch(type){
				case COL_AABB:
					return CheckCollision(aabb, other.sphere);
				case COL_SPHERE:
					return CheckCollision(sphere, other.aabb);
			}
		}
	}
};

#endif
