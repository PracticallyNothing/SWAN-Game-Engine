#ifndef SWAN_TRANSFORM_HPP
#define SWAN_TRANSFORM_HPP

#include "Maths/Matrix.hpp"
#include "Maths/Vector.hpp"

namespace SWAN
{
	/// A representation of a transformation.
	struct Transform {
		Transform(vec3 _pos = vec3(0), vec3 _rot = vec3(0),
		          vec3 _scale = vec3(1), Transform* _parent = NULL)
		    : pos(_pos), rot(_rot), scale(_scale), parent(_parent) {}
		~Transform() {}

		/// Get the transform's parent.
		inline Transform* getParent() { return parent; }

		/// Set the transform's parent.
		inline void setParent(Transform* t)
		{
			if(t->parent == this || t == this)
				return;
			else
				parent = t;
		}

		void lookAt(vec3 v)
		{
		}

		/// Calculate a Model matrix.
		mat4 getModel() const
		{
			mat4 posMat = getPosMat();
			mat4 scaleMat = getScaleMat();
			mat4 rotMat = getRotMat();
			mat4 parentMat = (parent == NULL ? mat4() : parent->getModel());

			return parentMat * posMat * rotMat * scaleMat;
		}

		/// Get an inverse of the Model matrix.
		inline mat4 getModel_inv() const { return Inverse(getModel()); }

		/// Calculate a position matrix.
		inline mat4 getPosMat() const { return Translate(pos); }

		/// Calculate a rotation matrix. @note The rotation is in the order Z * X * Y.
		inline mat4 getRotMat() const
		{
			mat4 rX = Rotate(rot.x, vec3(1, 0, 0));
			mat4 rY = Rotate(rot.y, vec3(0, 1, 0));
			mat4 rZ = Rotate(rot.z, vec3(0, 0, 1));

			//return rZ * rX * rY;
			return rY * rX * rZ;
		}

		/// Calculate a scale matrix.
		inline mat4 getScaleMat() const { return Scale(scale); }

		/// Get the forward direction for this transform.
		vec3 getForw() const { return vec4(0, 0, 1, 0) * Transpose(getModel()); }

		/// Get the upward direction for this transform.
		vec3 getUp() const { return vec4(0, 1, 0, 0) * Transpose(getModel()); }

		/// Get the right direction for this transform.
		vec3 getRight() const { return Cross(getUp(), getForw()); }

		/// Position.
		vec3 pos;
		/// Rotation.
		vec3 rot;
		/// Scale.
		vec3 scale;

		bool lookingAt = false;
		vec3 lookAtV;

		/// Parent transform.
		Transform* parent;
	};
} // namespace SWAN

#endif
