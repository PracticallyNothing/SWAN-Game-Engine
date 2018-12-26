#include "Basic.hpp"

#include "Maths/Vector.hpp"
#include "Utility/Math.hpp"
#include <limits>

namespace SWAN {
    // The code is a modified version of www.miguelcasillas.com/?p=43
    // (archive: https://web.archive.org/web/20160320085053/http://www.miguelcasillas.com/?p=43)
    /**
     * @brief Where does a point lie in relation to a plane?
     *
     * @note Planes don't actually have a thickness value, since they're only two-dimensional.
     *       What is meant here by thickness is how far away from the surface of the plane the point can be
     *       before it's considered to be lying on it.
     *
     * @param point Target point in 3D space to test.
     * @param p Plane to test against.
     * @param thickness Optional, how thick the plane is, default value is epsilon.
     *
     * @return HSResult::Front if the point is in front of the plane,
     *         HSResult::Back if it's behind,
     *         HSResult::On if it lies exactly on the plane.
     */
    HSResult HalfSpaceTest(vec3 point, Plane plane, double thickness){
	// Calculate the distance: dot product of the new vector with the plane's normal minus the offset.
	double distance = Dot(point, plane.normal) - plane.offset;

	// The point is in front of the plane.
	if(distance > thickness) return HSResult::Front;
	// The point is behind the plane.
	else if(distance < -thickness) return HSResult::Back;
	// If neither is true, then the point is on the plane.
	else return HSResult::On;
    }

    Intersection FindIntersection(Line line, Plane plane){
	bool arePerpendicular = Dot(plane.normal, line.vec) == 0;
	if(arePerpendicular)
	    return Intersection();

	double t = (Dot(plane.normal, plane.getPointOnPlane())
		    - Dot(plane.normal, line.point))
	    / Dot(plane.normal, line.vec);

	Intersection res;
	res.happened = true;
	res.point = line.point + line.vec * t;
	res.type = Intersection::Type::Point;
	return res;
    }
    Intersection FindIntersection(Ray ray, Plane plane){
	bool arePerpendicular = Dot(plane.normal, ray.dir) == 0;
	if(arePerpendicular)
	    return Intersection();

	double t = (Dot(plane.normal, plane.getPointOnPlane())
		    - Dot(plane.normal, ray.start))
	    / Dot(plane.normal, ray.dir);

	// A line would intersect, but a negative t means
	// that the intersection point is behind the ray.
	if(t < 0)
	    return Intersection();

	Intersection res;
	res.happened = true;
	res.point = ray.start + ray.dir * t;
	res.type = Intersection::Type::Point;
	return res;
    }
    Intersection FindIntersection(Segment segment, Plane plane) {
	bool arePerpendicular = Dot(plane.normal, segment.dir()) == 0;
	if(arePerpendicular)
	    return Intersection();

	double t = (Dot(plane.normal, plane.getPointOnPlane()) -
		    Dot(plane.normal, segment.start))
	    / Dot(plane.normal, segment.dir());

	// t must both be positive (so that the point of intersection isn't behind the starting point)
	// and not longer than the length of the segment (so that the intersection doesn't happen
	//                                                beyond the end point).
	if(t > 0 && t < segment.length()) {
	    Intersection res;
	    res.happened = true;
	    res.point = segment.start + segment.dir() * t;
	    res.type = Intersection::Type::Point;
	    return res;
	} else {
	    return Intersection();
	}
    }

    Intersection FindIntersection(AABB aabb, vec3 point) {
	Intersection res;

	res.happened =
	    Util::InRange(point.x, aabb.min.x, aabb.max.x) &&
	    Util::InRange(point.y, aabb.min.y, aabb.max.y) &&
	    Util::InRange(point.z, aabb.min.z, aabb.max.z);

	res.type = Intersection::Type::Point;
	res.point = point;

	return res;
    }
    Intersection FindIntersection(AABB aabb, Line line) {
    }
    Intersection FindIntersection(AABB aabb, Ray ray) {
    }
    Intersection FindIntersection(AABB aabb, Segment segment) {
	Intersection res;

	res.happened =
	    Util::RangesOverlap(aabb.min.x, aabb.max.x, segment.start.x, segment.end.x) &&
	    Util::RangesOverlap(aabb.min.y, aabb.max.y, segment.start.y, segment.end.y) &&
	    Util::RangesOverlap(aabb.min.z, aabb.max.z, segment.start.z, segment.end.z);

	res.point = (aabb.max - aabb.min) - (segment.end - segment.start);
	res.type = Intersection::Type::Point;

	return res;
    }
    Intersection FindIntersection(AABB aabb, Plane plane) {
    }

    Intersection FindIntersection(Sphere sphere, vec3 point) {
	Intersection res;

	res.happened = Length(sphere.center - point) < sphere.radius;
	res.point = point;
	res.type = Intersection::Type::Point;

	return res;
    }
    Intersection FindIntersection(Sphere sphere, Line line) {
    }
    Intersection FindIntersection(Sphere sphere, Ray ray) {
    }
    Intersection FindIntersection(Sphere sphere, Segment segment) {
    }
    Intersection FindIntersection(Sphere sphere, AABB aabb) {
	// Closest point from AABB to Sphere
	vec3 p(
	    std::max(aabb.min.x, std::min(sphere.center.x, aabb.max.x)),
	    std::max(aabb.min.y, std::min(sphere.center.y, aabb.max.y)),
	    std::max(aabb.min.z, std::min(sphere.center.z, aabb.max.z))
	);

	double dist = Length(p - sphere.center);

	Intersection res;
	res.happened = dist < sphere.radius;
	res.point = p;
	res.type = Intersection::Type::Point;
	return res;
    }

    Intersection FindIntersection(Triangle triangle, Line line) {
	Intersection i = FindIntersection(line, Plane(triangle));

	// If the plane the triangle lies on and the line don't intersect,
	// the line and the triangle can't either.
	if(!i.happened)
	    return Intersection();

	//if(i.point)
    }
    Intersection FindIntersection(Triangle triangle, Segment segment) {}
    Intersection FindIntersection(Triangle triangle, Plane plane) {}

    Intersection FindIntersection(Line l1, Line l2) {
    }
    Intersection FindIntersection(Triangle t1, Triangle t2) {
    }
    Intersection FindIntersection(Plane p1, Plane p2) {
    }
    Intersection FindIntersection(AABB a, AABB b) {
	Intersection res;

	res.happened =
	    Util::RangesOverlap(a.min.x, a.max.x, b.min.x, b.max.x) &&
	    Util::RangesOverlap(a.min.y, a.max.y, b.min.y, b.max.y) &&
	    Util::RangesOverlap(a.min.z, a.max.z, b.min.z, b.max.z);

	res.point = (a.max - a.min) - (b.max - b.min);
	res.type = Intersection::Type::Point;

	return res;
    
    }
    Intersection FindIntersection(Sphere a, Sphere b) {
	Intersection res;

	res.happened = Length(a.center - b.center) < a.radius + b.radius;
	res.point = b.center + (a.center - b.center) / 2;
	res.type = Intersection::Type::Point;

	return res;
    }
}
