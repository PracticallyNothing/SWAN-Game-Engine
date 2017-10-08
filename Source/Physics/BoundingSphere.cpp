#include "BoundingSphere.hpp"

#include <algorithm> // For std::max

#include "Core/Resources.hpp"    // For Resources::GetMesh(), Resources::GetShader()
#include "Rendering/Camera.hpp"  // For Camera
#include "Rendering/Mesh.hpp"    // For Mesh
#include "Rendering/Shader.hpp"  // For Shader

using std::max;
namespace SWAN {
	BoundingSphere ApplyTransform(BoundingSphere s, Transform t){
		// Offset the sphere's center by the Transform's position
		s.center += t.pos;

		// No rotation as this is a sphere

		// Change the sphere's radius based on the biggest scale
		s.radius *= max(t.scale.x, max(t.scale.y, t.scale.z));

		return s;
	}

	void Render(BoundingSphere sp, const Camera* cam, bool colliding){
		// Transform to populate
		Transform t;

		// Set the position to the sphere's center
		t.pos = sp.center - glm::vec3(sp.radius) * 0.5f;

		// Set the scale to the sphere's radius
		t.scale.x = sp.radius;
		t.scale.y = sp.radius;
		t.scale.z = sp.radius;

		Shader* s = Resources::GetShader("Physics");
		auto m = Resources::GetMesh("1x1 Sphere");

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
}
