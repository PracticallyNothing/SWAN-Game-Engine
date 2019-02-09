#ifndef SWAN_ENTITY_HPP
#define SWAN_ENTITY_HPP

#include "Maths/Vector.hpp"      // For vec3
#include "Physics/Transform.hpp" // For Transform
#include "Rendering/Mesh.hpp"    // For Mesh
#include "Rendering/Shader.hpp"  // For Shaders
#include "Rendering/Texture.hpp" // For Texture

#include <cstdint> // For std::uint32_t

namespace SWAN
{
	float Interpolate(float current, float goal, uint32_t deltaTimeMs)
	{
		float diff = goal - current;
		float dt = (float) deltaTimeMs / 1000;

		if(diff > dt)
			return current + dt;
		else if(diff < -dt)
			return current - dt;
		else
			return goal;
	}

	class Entity
	{
	  public:
		Entity() : mesh(NULL), tex(NULL) {}
		Entity(const Mesh* mesh, const Texture* tex) : mesh(mesh), tex(tex) {}

		~Entity() {}

		virtual void update(clock_t dt)
		{
			currVel.x = Interpolate(currVel.x, targetVel.x, dt);
			currVel.y = Interpolate(currVel.y, targetVel.y, dt);
			currVel.z = Interpolate(currVel.z, targetVel.z, dt);

			currAngVel.x = Interpolate(currAngVel.x, targetAngVel.x, dt);
			currAngVel.y = Interpolate(currAngVel.y, targetAngVel.y, dt);
			currAngVel.z = Interpolate(currAngVel.z, targetAngVel.z, dt);

			transform.pos += currVel;

			transform.rot.x += currAngVel.x;
			transform.rot.y += currAngVel.y;
			transform.rot.z += currAngVel.z;
		}

		virtual void render(Shader* s)
		{
			// s->SetMat4("transform", transform.getModel());
			// tex->bind();
			// s->renderMesh(*mesh);
			// mesh->
		}

		void setCurrVelX(float val) { currVel.x = val; }
		void setCurrVelY(float val) { currVel.y = val; }
		void setCurrVelZ(float val) { currVel.z = val; }

		void setTargetVelX(float val) { targetVel.x = val; }
		void setTargetVelY(float val) { targetVel.y = val; }
		void setTargetVelZ(float val) { targetVel.z = val; }

		void setCurrVel(vec3 v) { currVel = v; }
		void setTargetVel(vec3 v) { targetVel = v; }
		vec3 getCurrVel() { return currVel; }
		vec3 getTargetVel() { return targetVel; }

		void setCurrAngVelX(float val) { currAngVel.x = val; }
		void setCurrAngVelY(float val) { currAngVel.y = val; }
		void setCurrAngVelZ(float val) { currAngVel.z = val; }

		void setTargetAngVelX(float val) { targetAngVel.x = val; }
		void setTargetAngVelY(float val) { targetAngVel.y = val; }
		void setTargetAngVelZ(float val) { targetAngVel.z = val; }

		void setCurrAngVel(vec3 v) { currAngVel = v; }
		void setTargetAngVel(vec3 v) { targetAngVel = v; }
		vec3 getCurrAngVel() { return currAngVel; }
		vec3 getTargetAngVel() { return targetAngVel; }

		const Mesh* getMesh() const { return mesh; }
		void setMesh(const Mesh* m) { mesh = m; }

		const Texture* getTexture() const { return tex; }
		void setTexture(const Texture* t) { tex = t; }

		Transform getTransform() { return transform; }
		Transform& getTransform_ref() { return transform; }
		const Transform* getTransform_ptr() { return &transform; }
		void setTransform(Transform t) { transform = t; }

		inline void scaleByX(float amt) { transform.scale.x += amt; }
		inline void scaleByY(float amt) { transform.scale.y += amt; }
		inline void scaleByZ(float amt) { transform.scale.z += amt; }
		inline void scaleByVec(vec3 vec) { transform.scale += vec; }

		inline void scaleToX(float amt) { transform.scale.x = amt; }
		inline void scaleToY(float amt) { transform.scale.y = amt; }
		inline void scaleToZ(float amt) { transform.scale.z = amt; }
		inline void scaleToVec(vec3 vec) { transform.scale = vec; }

	  protected:
		const Mesh* mesh;
		const Texture* tex;
		Transform transform;

		vec3 currVel, targetVel, currAngVel, targetAngVel;
	};
} // namespace SWAN

#endif
