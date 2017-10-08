#include "CheckCollision.hpp"

#include "Utility/Math.hpp"
#include <algorithm> // For std::max<T>(), std::min<T>()
#include <cmath> // For std::sqrt()

using std::sqrt;
using std::min;
using std::max;

using glm::length;
using glm::normalize;

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace SWAN {
	/*
	Collision CheckCollision(BoundingSphere s, vec3 p){
		Collision res;

		res.happened = length(s.center - p) < s.radius;
		res.distance = length(s.center - p) - s.radius;

		return res;
	}

	Collision CheckCollision(BoundingSphere a, BoundingSphere b){
		Collision res;

		res.happened = length(a.center - b.center) < a.radius + b.radius;
		res.distance = length(a.center - b.center) - (a.radius + b.radius);

		return res;
	}
	*/

	Collision CheckCollision(AABB a, LineSegment b){
		Collision res;

		res.happened =
			Util::RangesOverlap(a.min.x, a.max.x, b.start.x, b.end.x) &&
			Util::RangesOverlap(a.min.y, a.max.y, b.start.y, b.end.y) &&
			Util::RangesOverlap(a.min.z, a.max.z, b.start.z, b.end.z);

		res.distance = length((a.max - a.min) - (b.end - b.start));

		return res;
	}

	Collision CheckCollision(AABB a, AABB b){
		Collision res;

		res.happened =
			Util::RangesOverlap(a.min.x, a.max.x, b.min.x, b.max.x) &&
			Util::RangesOverlap(a.min.y, a.max.y, b.min.y, b.max.y) &&
			Util::RangesOverlap(a.min.z, a.max.z, b.min.z, b.max.z);

		res.distance = length((a.max - a.min) - (b.max - b.min));

		return res;
	}

	/*
	Collision CheckCollision(BoundingSphere s, AABB b){
		// Closest point from AABB to Sphere
		glm::vec3 p(max(b.min.x, min(s.center.x, b.max.x)),
				max(b.min.y, min(s.center.y, b.max.y)),
				max(b.min.z, min(s.center.z, b.max.z)));

		double dist =
			sqrt((p.x - s.center.x) * (p.x - s.center.x) +
				 (p.y - s.center.y) * (p.y - s.center.y) +
				 (p.z - s.center.z) * (p.z - s.center.z));

		return Collision { dist < s.radius, (float) dist };
	}
	*/
}
