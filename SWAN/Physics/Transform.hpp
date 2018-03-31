#ifndef SWAN_TRANSFORM_HPP
#define SWAN_TRANSFORM_HPP

#include "../Maths/Vector.hpp"
#include "../Maths/Matrix.hpp"

namespace SWAN {
	struct Transform {
		Transform(vec3 _pos = vec3(0), vec3 _rot = vec3(0),
				  vec3 _scale = vec3(1), Transform* _parent = NULL)
			: pos(_pos), rot(_rot), scale(_scale), parent(_parent) {}
		~Transform() {}

		inline Transform* getParent() { return parent; }

		inline void setParent(Transform* t) {
			if(t->parent == this || t == this)
				return;
			else
				parent = t;
		}

		mat4 getModel() const {
			mat4 posMat   = getPosMat();
			mat4 scaleMat = getScaleMat();
			mat4 rotMat   = getRotMat();
			mat4 parentMat =
				(parent == NULL ? mat4() : parent->getModel());

			return parentMat * posMat * rotMat * scaleMat;
		}

		inline mat4 getModel_inv() const { return Inverse(getModel()); }

		inline mat4 getPosMat() const { return Translate(pos); }
		inline mat4 getRotMat() const {
			mat4 rotMatX = Rotate(rot.x, vec3(1, 0, 0));
			mat4 rotMatY = Rotate(rot.y, vec3(0, 1, 0));
			mat4 rotMatZ = Rotate(rot.z, vec3(0, 0, 1));

			return rotMatZ * rotMatX * rotMatY;
		}
		inline mat4 getScaleMat() const { return Scale(scale); }

		vec3 getForw() const { return vec4(0, 0, 1, 0) * getRotMat(); }
		vec3 getUp() const { return vec4(0, 1, 0, 0) * getRotMat(); }

		vec3 getRight() const { return Cross(getForw(), getUp()); }

		vec3 pos;
		vec3 rot;
		vec3 scale;

		Transform* parent;
	};
} // namespace SWAN

#endif
