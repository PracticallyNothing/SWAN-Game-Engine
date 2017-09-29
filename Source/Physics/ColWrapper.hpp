#ifndef COL_WRAPPER_HPP
#define COL_WRAPPER_HPP

#include "AABB.hpp"
//#include "BoundingSphere.hpp"
#include "CheckCollision.hpp"

struct ColWrapper {
	ColWrapper ()                 : type(COL_NONE)                 {}
	ColWrapper (glm::vec3 point)  : type(COL_POINT) , point(point) {}
	ColWrapper (Ray ray)          : type(COL_RAY)   , ray(ray)     {}
	ColWrapper (Plane plane)      : type(COL_PLANE) , plane(plane) {}
	ColWrapper (AABB aabb)        : type(COL_AABB)  , aabb(aabb)   {}
	//ColWrapper (BoundingSphere s) : type(COL_SPHERE), sphere(s)    {}

	ColWrapper(const ColWrapper& c);
	ColWrapper(ColWrapper&& c);

	ColWrapper& operator=(const ColWrapper& c);
	ColWrapper& operator=(ColWrapper&& c);

	ColWrapper& operator=(glm::vec3 point)  { type = COL_POINT;  this->point = point; return *this; }
	ColWrapper& operator=(Ray ray)          { type = COL_RAY;    this->ray = ray;     return *this; }
	ColWrapper& operator=(Plane plane)      { type = COL_PLANE;  this->plane = plane; return *this; }
	ColWrapper& operator=(AABB aabb)        { type = COL_AABB;   this->aabb = aabb;   return *this; }
	//ColWrapper& operator=(BoundingSphere s) { type = COL_SPHERE; this->sphere = s;    return *this; }

	enum Type {
		COL_NONE,  // No collision object

		COL_POINT, // Point in 3D space
		COL_RAY,   // Infinite line
		COL_PLANE, // Plane

		COL_AABB,  // Axis Aligned Bounding Box
		//COL_SPHERE // Bounding sphere
	} type;

	union {
		glm::vec3 point;
		Ray ray;
		Plane plane;

		AABB aabb;
		//BoundingSphere sphere;
	};

	Collision checkCollision(const ColWrapper& other);
};

#endif
