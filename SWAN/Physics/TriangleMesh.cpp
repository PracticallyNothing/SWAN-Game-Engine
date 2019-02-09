#include "TriangleMesh.hpp"

namespace SWAN
{
	AABB TriangleMesh::GetAABB() const
	{
		SWAN::vec3 min(0, 0, 0), max(0, 0, 0);

		for(const auto& t : Triangles) {
			for(const auto& v : t.points) {
				min.x = std::min<double>(v.x, min.x);
				min.y = std::min<double>(v.y, min.y);
				min.z = std::min<double>(v.z, min.z);

				max.x = std::max<double>(v.x, max.x);
				max.y = std::max<double>(v.y, max.y);
				max.z = std::max<double>(v.z, max.z);
			}
		}

		AABB res;
		res.min = min;
		res.max = max;
		return res;
	}

	Intersection FindIntersection(TriangleMesh a, TriangleMesh b)
	{
		if(!FindIntersection(a.GetAABB(), b.GetAABB())) {
			return Intersection();
		}
	}
} // namespace SWAN
