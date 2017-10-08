#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/fwd.hpp> // For glm::vec3

#include "Physics/AABB.hpp" // For AABB

struct Octree {

	struct Node {
		AABB bounds;
		unsigned treshold;
		Node* children;
	};

	Node root;
};

class Scene {

};

#endif
