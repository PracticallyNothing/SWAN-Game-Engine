#ifndef SWAN_PHYSICS_BASIC_HPP
#define SWAN_PHYSICS_BASIC_HPP

#include "Maths/Vector.hpp"
#include "Transform.hpp"
#include <limits>
#include <vector>

namespace SWAN {
    /// An infinitely long line.
    struct Line { vec3 point, vec; };
    /// A one-directional line.
    struct Ray {
	/// Constructs a default ray.
	Ray() {}
	/// Constructs a ray with a starting point and a direction.
	Ray(vec3 start, vec3 dir) : start(start), dir(Normalized(dir)) {}

	/// Starting point/Source of ray.
	vec3 start;
	/// Direction of the ray, must be normalized.
	vec3 dir;
    };
    /// A line with a limited length.
    struct Segment {
	/// Constructs a default line segment.
	Segment() {}
	/// Constructs a line segment with a starting and ending point.
	Segment(vec3 start, vec3 end) : start(start), end(end) {}
	/// Constructs a line segment with a starting point, a direction and a length.
	Segment(vec3 start, vec3 dir, double length) : start(start), end(start + Normalized(dir) * length) {}

	vec3 start, end;
	double length() { return Length(end - start); }
	vec3 dir() { return Normalized(end - start); }
    };

    struct Triangle {
	/// Default constructor.
	Triangle() {}

	/// Constructs a triangle with 3 points and calculates the normal.
	Triangle(vec3 a, vec3 b, vec3 c) {
	    points[0] = a;
	    points[1] = b;
	    points[2] = c;

	    normal = Normalized(Cross(b - a, c - a));
	}

	/// Constructs a triangle with a given normal and 3 points.
	Triangle(vec3 a, vec3 b, vec3 c, vec3 normal) {
	    points[0] = a;
	    points[1] = b;
	    points[2] = c;
	    
	    this->normal = Normalized(normal);
	}

	vec3 points[3];
	vec3 normal;
    };

    /// An axis-aligned bounding box.
    struct AABB {
	AABB() {}
	AABB(vec3 min, vec3 max) : min(min), max(max) {}
	AABB(std::vector<vec3> points)
	{
	    min = max = points[0];
	    for(auto v : points)
	    {
		if(v.x < min.x) min.x = v.x;
		else if(v.x > max.x) max.x = v.x;

		if(v.y < min.y) min.y = v.y;
		else if(v.y > max.y) max.y = v.y;

		if(v.z < min.z) min.z = v.z;
		else if(v.z > max.z) max.z = v.z;
	    }
	}

	vec3 min, max;

	/// Get the center of the AABB.
	vec3 center() const { return max - min; }

	/// Get the length of the AABB along the X axis.
	inline double XLen() const { return max.x - min.x; }

	/// Get the length of the AABB along the Y axis.
	inline double YLen() const { return max.y - min.y; }

	/// Get the length of the AABB along the Z axis.
	inline double ZLen() const { return max.z - min.z; }

	/// Gets the volume of the bounding box.
	inline double Volume() const { return XLen() * YLen() * ZLen(); }

	void ApplyTransform(Transform t) {
	    auto rotMat = t.getRotMat();
	    min = vec4(min, 1) * rotMat;
	    max = vec4(max, 1) * rotMat;

	    min += t.pos;
	    max += t.pos;

	    vec3 tmp1 = min, tmp2 = max;

	    min.x = std::min(tmp1.x, tmp2.x);
	    min.y = std::min(tmp1.y, tmp2.y);
	    min.z = std::min(tmp1.z, tmp2.z);

	    max.x = std::max(tmp1.x, tmp2.x);
	    max.y = std::max(tmp1.y, tmp2.y);
	    max.z = std::max(tmp1.z, tmp2.z);
	}
    };

    struct Plane {
	/// Default constructor. Creates a plane with no normal or offset.
	Plane() {}

	/// Constructs a plane with a given normal, but no offset from origin (0,0,0).
	Plane(vec3 normal) : normal(Normalized(normal)) {}

	/// Constructs a plane with a given normal and an offset from origin (0,0,0).
	Plane(vec3 normal, double offset) : normal(Normalized(normal)), offset(offset) {}

	/// Constructs a plane from a normal and a point on the plane.
	Plane(vec3 normal, vec3 point) : normal(Normalized(normal)), offset(Dot(normal, point)) {}

	/// Constructs a plane from a triangle.
	Plane(Triangle tri) : normal(tri.normal), offset(Dot(normal, tri.points[0])) {}

	/// Normal of the plane.
	vec3 normal;
	/// Offset from origin (0,0,0).
	double offset;

	/// Creates a point that lies on the plane.
	inline vec3 getPointOnPlane() {return normal * offset;}

    };

    struct Sphere {
	/// Center point of the sphere.
	vec3 center;

	/// Radius of the sphere.
	float radius;

	void ApplyTransform(Transform t) {
	    // Offset the sphere's center by the Transform's position
	    center += t.pos;
	    // Change the sphere's radius based on the biggest scale
	    radius *= std::max(t.scale.x, std::max(t.scale.y, t.scale.z));
	}
    };

    struct Intersection {
	/// Default constructor. Creates an Intersection that hasn't occured (happened = false).
	Intersection() {}

	/// Has there been an intersection?
	bool happened = false;

	/// Has there been an intersection?
	operator bool() const { return happened; }

	/// Type of intersection.
	enum class Type { None, Point, Line, Ray, Triangle, Plane } type = Type::None;

	union {
	    vec3 point;
	    Line line;
	    Ray ray;
	    Triangle triangle;
	    Plane plane;
	};
    };

    // ---------------------------------------------------------------------------------------------------------- //
    // ---------------------------------------------------------------------------------------------------------- //
    // ---------------------------------------------------------------------------------------------------------- //

    /// Enumeration for HalfSpaceTest() result.
    enum class HSResult {
	/// The point is in front of the plane.
	Front,
	/// The point is behind the plane.
	Back,
	/// The point lies exactly on the plane.
	On
    };

    // The code is a modified version of www.miguelcasillas.com/?p=43
    // (archive: https://web.archive.org/web/20160320085053/http://www.miguelcasillas.com/?p=43)
    /**
     * @brief Where does a point lie in relation to a plane?
     *
     * @note Planes don't actually have a thickness value, since they're only two-dimensional.
     *       What is meant here by thickness is how far away from the surface of the plane the point can be
     *       so that it's considered to be lying on it.
     *
     * @param point Target point in 3D space to test.
     * @param p Plane to test against.
     * @param thickness Optional, how thick the plane is, default value is epsilon.
     *
     * @return HSResult::Front if the point is in front of the plane,
     *         HSResult::Back if it's behind,
     *         HSResult::On if it lies exactly on the plane.
     */
    extern HSResult HalfSpaceTest(vec3 point, Plane plane, double thickness = std::numeric_limits<double>::epsilon());

    /// Finds the intersection between a line and a plane (if it exists). Returns a SWAN::Intersection.
    extern Intersection FindIntersection(Line line, Plane plane);
    /// Finds the intersection between a ray and a plane (if it exists). Returns a SWAN::Intersection.
    extern Intersection FindIntersection(Ray ray, Plane plane);
    /// Finds the intersection between a line segment and a plane (if it exists). Returns a SWAN::Intersection.
    extern Intersection FindIntersection(Segment segment, Plane plane);

    extern Intersection FindIntersection(AABB aabb, vec3 point);
    extern Intersection FindIntersection(AABB aabb, Line line);
    extern Intersection FindIntersection(AABB aabb, Ray ray);
    extern Intersection FindIntersection(AABB aabb, Segment segment);
    extern Intersection FindIntersection(AABB aabb, Plane plane);

    extern Intersection FindIntersection(Sphere sphere, vec3 point);
    extern Intersection FindIntersection(Sphere sphere, Line line);
    extern Intersection FindIntersection(Sphere sphere, Ray ray);
    extern Intersection FindIntersection(Sphere sphere, Segment segment);
    extern Intersection FindIntersection(Sphere sphere, AABB aabb);

    extern Intersection FindIntersection(Triangle triangle, Line line);
    extern Intersection FindIntersection(Triangle triangle, Ray ray);
    extern Intersection FindIntersection(Triangle triangle, Segment segment);
    extern Intersection FindIntersection(Triangle triangle, Plane plane);

    extern Intersection FindIntersection(Line l1, Line l2);
    extern Intersection FindIntersection(Triangle t1, Triangle t2);
    extern Intersection FindIntersection(Plane p1, Plane p2);
    extern Intersection FindIntersection(AABB a1, AABB a2);
    extern Intersection FindIntersection(Sphere s1, Sphere s2);

    // ---------------------------------------------------------------------------------------------------------- //
    // ---------------------------------------------------------------------------------------------------------- //
    // ---------------------------------------------------------------------------------------------------------- //

    inline Intersection FindIntersection(vec3 point, Sphere sphere) { return FindIntersection(sphere, point); }
    inline Intersection FindIntersection(Line line, Sphere sphere) { return FindIntersection(sphere, line); }
    inline Intersection FindIntersection(Ray ray, Sphere sphere) { return FindIntersection(sphere, ray); }
    inline Intersection FindIntersection(Segment segment, Sphere sphere) { return FindIntersection(sphere, segment); }
    inline Intersection FindIntersection(AABB aabb, Sphere sphere) { return FindIntersection(sphere, aabb); }

    inline Intersection FindIntersection(vec3 point, AABB aabb) { return FindIntersection(aabb, point); }
    inline Intersection FindIntersection(Line line, AABB aabb) { return FindIntersection(aabb, line); }
    inline Intersection FindIntersection(Ray ray, AABB aabb) { return FindIntersection(aabb, ray); }
    inline Intersection FindIntersection(Segment segment, AABB aabb) { return FindIntersection(aabb, segment); }
    inline Intersection FindIntersection(Plane plane, AABB aabb) { return FindIntersection(aabb, plane); }

    /// Finds the intersection between a line and a plane (if it exists). Returns a SWAN::Intersection.
    inline Intersection FindIntersection(Plane plane, Line line) { return FindIntersection(line, plane); }
    /// Finds the intersection between a ray and a plane (if it exists). Returns a SWAN::Intersection.
    inline Intersection FindIntersection(Plane plane, Ray ray) { return FindIntersection(ray, plane); }
    /// Finds the intersection between a line segment and a plane (if it exists). Returns a SWAN::Intersection.
    inline Intersection FindIntersection(Plane plane, Segment segment) { return FindIntersection(segment, plane); }

    inline Intersection FindIntersection(Line line, Triangle triangle) { return FindIntersection(triangle, line); }
    inline Intersection FindIntersection(Ray ray, Triangle triangle) { return FindIntersection(triangle, ray); }
    inline Intersection FindIntersection(Segment segment, Triangle triangle) { return FindIntersection(triangle, segment); }

}

#endif
