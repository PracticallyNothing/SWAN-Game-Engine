#include "CheckCollision.hpp"

#include "../Utility/Math.hpp"

using glm::length;
using glm::normalize;

using glm::vec3;
using glm::vec4;
using glm::mat4;

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

Collision CheckCollision(BoundingSphere s, AABB b){
	return CheckCollision(
		b,
		LineSegment{ 
			s.center, 
			s.center + s.radius * normalize(s.center - (b.min + (b.max - b.min) / 2.0f)) }
	);
}
