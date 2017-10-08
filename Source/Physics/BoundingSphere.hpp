#ifndef SWAN_BOUNDING_SPHERE_HPP
#define SWAN_BOUNDING_SPHERE_HPP

#include <glm/glm.hpp>

#include "Transform.hpp" // For Transform

#include "Rendering/Camera.hpp"  // For Camera
#include "Rendering/Shader.hpp"  // For Shader

namespace SWAN {
	struct BoundingSphere {
		glm::vec3 center;
		float radius;
	};

	BoundingSphere ApplyTransform(BoundingSphere s, Transform t);

	void Render(BoundingSphere sphere, const Camera* cam, bool colliding = false);
}

#endif
