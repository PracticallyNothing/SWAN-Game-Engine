#include "AABB.hpp"

#include <algorithm>  // For std::max(), std::min()

#include "../Core/Resources.hpp"    // For Resources::GetMesh(), Resources::GetShader()
#include "../Rendering/Mesh.hpp"    // For Mesh
#include "../Utility/Debug.hpp"     // For DEBUG_OUT()
#include "../Utility/StreamOps.hpp" // For Util::StreamOps::*

using namespace Util::StreamOps;

using glm::length;

using glm::vec3;
using glm::vec4;
using glm::mat4;

using std::min;
using std::max;

void MakeMinMax(vec3& a, vec3& b) {
	vec3 tmp1 = a, tmp2 = b;

	a.x = min(tmp1.x, tmp2.x);
	a.y = min(tmp1.y, tmp2.y);
	a.z = min(tmp1.z, tmp2.z);

	b.x = max(tmp1.x, tmp2.x);
	b.y = max(tmp1.y, tmp2.y);
	b.z = max(tmp1.z, tmp2.z);
}

AABB ApplyTransform(AABB box, Transform t) {
	box.min = vec3(vec4(box.min, 1) * t.getRotMat());
	box.max = vec3(vec4(box.max, 1) * t.getRotMat());

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

	Shader* s = Resources::GetShader("Physics");
	auto m = Resources::GetMesh("1x1 Cube");

	s->use();
	{
		s->setUniformData("perspective", cam->getPerspective());
		s->setUniformData("view", cam->getView());
		s->setUniformData("transform", t);
		s->setUniformData("colliding", colliding);

		m->renderWireframe();
	}
	s->unuse();
}
