#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct Transform {
	Transform(glm::vec3  _pos    = glm::vec3(0),
			glm::vec3  _rot    = glm::vec3(0),
			glm::vec3  _scale  = glm::vec3(1),
			Transform* _parent = NULL) 
		: pos(_pos), rot(_rot), scale(_scale), parent(_parent) {}
	~Transform(){}

	inline Transform* getParent(){
		return parent;
	}

	inline void setParent(Transform* t){
		if(t->parent == this || t == this)
			return;
		else
			parent = t;
	}

	glm::mat4 getModel() const {
		glm::mat4 posMat = getPosMat();
		glm::mat4 scaleMat = getScaleMat();
		glm::mat4 rotMat = getRotMat();
		glm::mat4 parentMat = (parent == NULL ? glm::mat4(1.0f) : parent->getModel());

		return parentMat * posMat * rotMat * scaleMat;
	}

	inline glm::mat4 getModel_inv() const { return glm::inverse(getModel()); }

	inline glm::mat4 getPosMat()   const { return glm::translate(pos); }
	inline glm::mat4 getRotMat()   const { 
		glm::mat4 rotMatX = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotMatY = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotMatZ = glm::rotate(rot.z, glm::vec3(0, 0, 1));

		return rotMatZ * 
			   rotMatY * 
			   rotMatX;
	}
	inline glm::mat4 getScaleMat() const { return glm::scale(scale); }

	glm::vec3 getForw()  const { return glm::vec3(glm::vec4(0, 0, 1, 0) * getRotMat()); }
	glm::vec3 getUp()    const { return glm::vec3(glm::vec4(0, 1, 0, 0) * getRotMat()); }
	glm::vec3 getRight() const { return glm::cross(getForw(), getUp()); }

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;

	Transform* parent;
};

#endif
