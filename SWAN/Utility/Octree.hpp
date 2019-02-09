#ifndef SWAN_OCTREE_HPP
#define SWAN_OCTREE_HPP

#include <memory> // For std::unique_ptr<T>
#include <vector> // For std::vector<T>

#include "Physics/Basic.hpp" // For SWAN::AABB

namespace SWAN
{
	template <typename T, unsigned Threshold>
	struct Octree {
		enum ObjectSector {
			/// Inside the main sector
			S_Main = 0,

			S_TopLeftFront,
			S_TopLeftBack,
			S_TopRightFront,
			S_TopRightBack,

			S_BottomLeftFront,
			S_BottomLeftBack,
			S_BottomRightFront,
			S_BottomRightBack,
		};

		struct Node {
			Node* parent = nullptr;
			Node* children = nullptr;
			AABB box;

			struct Object {
				const T* data;
				double x, y, z;
			};
			std::vector<Object> objects;

			~Node();

			bool isRoot() { return !parent; }
			bool hasChildren() { return children; }

			ObjectSector getObjectSector(double x, double y, double z);
			void split();
			void add(Object o);
		};

		void add(const T* data, double x, double y, double z)
		{
			root->add({ data, x, y, z });
		}

		std::unique_ptr<Node> root;
	};
} // namespace SWAN

#endif // SWAN_OCTREE_HPP
