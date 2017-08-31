#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <cstdint>
#include <vector>
#include "../../Logic/Entity.hpp"

static std::vector<uint64_t> OtherFreeIDs;
static uint64_t NextFreeID = 0;

uint64_t GetNextFreeID() {
	if (OtherFreeIDs.size()) {
		uint64_t res = OtherFreeIDs.back();
		OtherFreeIDs.pop_back();
		return res;
	}
	return NextFreeID++;
}

class Node {
	public:
		Node(Node* parent = nullptr) : ID(GetNextFreeID()), parent(parent) {}

		~Node() {
			passToParent();
			OtherFreeIDs.push_back(ID);
		}

		void passToParent() {
			if (!parent) return;

			if (passChildren)
				for (auto& n : children) {
					if (std::find_if(
							parent->children.begin(), 
							parent->children.end(),
							[this](Node& n) {
								return n.getID() == this->getID();
							}) == parent->children.end())
					parent->children.push_back(n);
				}
			if (passEntities)
				for (auto& e : entities) parent->entities.push_back(e);
		}

		bool operator==(const Node& other) const { return ID == other.ID; }
		bool operator!=(const Node& other) const { return ID != other.ID; }

		Node* parent;
		bool passChildren = false, passEntities = false;

		std::vector<Node&> children;
		std::vector<Entity&> entities;

		uint64_t getID() { return ID; }

		bool render = true;

	private:
		uint64_t ID;
};

class Scene {
	public:
		Scene();
		~Scene();

		const Node& getRootNode(){ return root; }
	private:
		Node root;
};

#endif
