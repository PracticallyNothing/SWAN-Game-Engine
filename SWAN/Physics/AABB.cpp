#include "AABB.hpp"

#include <algorithm> // For std::max(), std::min()

#include "Core/Resources.hpp" // For SWAN::Res::GetMesh(), SWAN::Res::GetShader()
#include "Maths/Vector.hpp"
#include "Rendering/Mesh.hpp" // For Mesh

using SWAN::vec3;

using std::max;
using std::min;

void MakeMinMax(vec3& a, vec3& b) {
	vec3 tmp1 = a, tmp2 = b;

	a.x = min(tmp1.x, tmp2.x);
	a.y = min(tmp1.y, tmp2.y);
	a.z = min(tmp1.z, tmp2.z);

	b.x = max(tmp1.x, tmp2.x);
	b.y = max(tmp1.y, tmp2.y);
	b.z = max(tmp1.z, tmp2.z);
}

namespace SWAN {
bool PointIsInsideAABB(AABB box, vec3 point) {
	return box.min.x >= point.x &&
	       box.min.y >= point.y &&
	       box.min.z >= point.z &&
	       box.max.x <= point.x &&
	       box.max.y <= point.y &&
	       box.max.z <= point.z;
}

AABB ApplyTransform(AABB box, Transform t) {
	auto rotMat = t.getRotMat();
	box.min     = vec4(box.min, 1) * rotMat;
	box.max     = vec4(box.max, 1) * rotMat;

	box.min += t.pos;
	box.max += t.pos;

	MakeMinMax(box.min, box.max);

	return box;
}

void Render(AABB box, const Camera* cam, bool colliding) {
	Transform t;

	// Set the transform position to the center of the AABB
	t.pos = box.max - (box.max - box.min) * 0.5f;

	// Rotation is not set, the box is axis-aligned

	// Set the scale of the transform to the size of the AABB
	// (assumes that the mesh used to render has a size of 1)
	t.scale.x = box.max.x - box.min.x;
	t.scale.y = box.max.y - box.min.y;
	t.scale.z = box.max.z - box.min.z;

	Shader* s = SWAN::Res::GetShader("Physics");
	auto m    = SWAN::Res::GetMesh("1x1 Cube");

	std::vector<ShaderUniform> uniforms = { { "perspective",
		                                      cam->getPerspective() },
		                                    { "view", cam->getView() },
		                                    { "transform", t },
		                                    { "colliding", colliding } };

	s->setUniforms(uniforms);

	s->use();
	s->renderWireframeMesh(*m);
	s->unuse();
}
} // namespace SWAN
