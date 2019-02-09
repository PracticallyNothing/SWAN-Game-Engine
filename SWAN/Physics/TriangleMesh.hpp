#ifndef SWAN_PHYS_TRIANGLE_MESH_HPP
#define SWAN_PHYS_TRIANGLE_MESH_HPP

#include "Basic.hpp"

#include "Core/Defs.hpp"
#include "Maths/Vector.hpp"

namespace SWAN
{
	struct TriangleMesh {
		Vector<Triangle> Triangles;
		AABB GetAABB() const;
	};

	extern Intersection FindIntersection(TriangleMesh a, TriangleMesh b);
} // namespace SWAN

#endif
