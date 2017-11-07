#include "Octree.hpp"

#include <stack> // For std::stack<T>

namespace SWAN {
template <typename T, unsigned Threshold>
Octree<T, Threshold>::Node::~Node() {
	std::stack<Node*> s;
	s.push(this);

	while(this->children) {
		for(int i = 0; i < 8; i++) {
			if(s.top()->children[i]->children) {
				s.push(&s.top()->children[i]);
				break;
			}
		}
		delete[] s.top()->children;
		s.top()->children = nullptr;
		s.pop();
	}
}

template <typename T, unsigned Threshold>
typename Octree<T, Threshold>::ObjectSector
Octree<T, Threshold>::Node::getObjectSector(double x, double y, double z) {
	if(!children)
		return Octree::S_Main;

	double centerX = box.min.x + (box.max.x - box.min.x) / 2,
	       centerY = box.min.y + (box.max.y - box.min.y) / 2,
	       centerZ = box.min.z + (box.max.z - box.min.z) / 2;

	if(y <= centerY) {
		if(x <= centerX) {
			if(z <= centerZ)
				return Octree::S_BottomLeftFront;
			else if(z > centerZ)
				return Octree::S_BottomLeftBack;
		} else if(x > centerX) {
			if(z <= centerZ)
				return Octree::S_BottomRightFront;
			else if(z > centerZ)
				return Octree::S_BottomRightBack;
		}
	} else if(y > centerY) {
		if(x <= centerX) {
			if(z <= centerZ)
				return Octree::S_TopLeftFront;
			else if(z > centerZ)
				return Octree::S_TopLeftBack;
		} else if(x > centerX) {
			if(z <= centerZ)
				return Octree::S_TopRightFront;
			else if(z > centerZ)
				return Octree::S_TopRightBack;
		}
	}
}

template <typename T, unsigned Threshold>
void Octree<T, Threshold>::Node::split() {
	children = new Node[8];

	double minX = box.min.x,
	       minY = box.min.y,
	       minZ = box.min.z;

	double maxX = box.max.x,
	       maxY = box.max.y,
	       maxZ = box.max.z;

	double centerX = minX + (maxX - minX) / 2,
	       centerY = minY + (maxY - minY) / 2,
	       centerZ = minZ + (maxZ - minZ) / 2;

	// --------- Top Front ----------------------------
	children[Octree::S_TopLeftFront - 1].box= {
		glm::vec3(minX, centerY, minZ),
		glm::vec3(centerX, maxY, centerZ)
	};

	children[Octree::S_TopRightFront - 1].box = {
		glm::vec3(centerX, centerY, minZ),
		glm::vec3(maxX, maxY, centerZ)
	};

	// --------- Top Back ----------------------------
	children[Octree::S_TopLeftBack - 1].box = {
		glm::vec3(minX, centerY, centerZ),
		glm::vec3(centerX, maxY, maxZ)
	};

	children[Octree::S_TopRightBack - 1].box = {
		glm::vec3(centerX, centerY, centerZ),
		glm::vec3(maxX, maxY, maxZ)
	};

	// --------- Bottom Front ----------------------------
	children[Octree::S_BottomLeftFront - 1].box = {
		glm::vec3(minX, minY, minZ),
		glm::vec3(centerX, centerY, centerZ)
	};

	children[Octree::S_BottomRightFront - 1].box = {
		glm::vec3(centerX, minY, minZ),
		glm::vec3(maxX, centerY, centerZ)
	};

	// --------- Bottom Back ----------------------------
	children[Octree::S_BottomLeftBack - 1].box = {
		glm::vec3(minX, minY, centerZ),
		glm::vec3(centerX, centerY, maxZ)
	};

	children[Octree::S_BottomRightBack - 1].box = {
		glm::vec3(centerX, minY, centerZ),
		glm::vec3(maxX, centerY, maxZ)
	};

	for(Object o : objects) {
		children[getObjectSector(o.x, o.y, o.z) - 1].objects.push_back(o);
	}
	objects.clear();
	objects.size_to_fit();
}

template <typename T, unsigned Threshold>
void Octree<T, Threshold>::Node::add(Object o) {
	if(objects.size() + 1 > Threshold)
		split();

	ObjectSector s = GetObjectSector(o.x, o.y, o.z);

	if(s == S_Main)
		objects.push_back(o);
	else
		children[s - 1].add(o);
}
} // namespace SWAN
