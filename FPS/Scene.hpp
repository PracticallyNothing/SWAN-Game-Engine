#ifndef SCENE_HPP
#define SCENE_HPP

#include <bullet/btBulletDynamicsCommon.h>

#include "SWAN/Physics/Basic.hpp"
#include "SWAN/Physics/Transform.hpp"
#include "SWAN/Rendering/Camera.hpp"
#include "SWAN/Rendering/Mesh.hpp"
#include "SWAN/Rendering/Texture.hpp"

class Entity
{
  public:
	Entity(const SWAN::Mesh* mesh, const SWAN::Texture* tex)
	    : Mesh(mesh), Texture(tex)
	{
		UpdateAABB();
	}

  private:
	void UpdateAABB()
	{
		if(!Mesh || Mesh->GetPoints().empty()) {
			AABB.min = (AABB.max = SWAN::vec3(0, 0, 0));
			return;
		}

		const auto& verts = Mesh->GetPoints();
		SWAN::vec3 min(verts[0]), max(verts[0]);

		for(const auto& v : verts) {
			min.x = std::min<double>(v.x, min.x);
			min.y = std::min<double>(v.y, min.y);
			min.z = std::min<double>(v.z, min.z);

			max.x = std::max<double>(v.x, max.x);
			max.y = std::max<double>(v.y, max.y);
			max.z = std::max<double>(v.z, max.z);
		}

		AABB.min = min;
		AABB.max = max;
	}

	const SWAN::Mesh* Mesh;
	const SWAN::Texture* Texture;
	SWAN::Transform Transform;
	SWAN::AABB AABB;
	btCollisionShape* shape;
};

class Scene
{
  public:
	void Render();
	SWAN::Camera Camera;

  private:
};

#endif
