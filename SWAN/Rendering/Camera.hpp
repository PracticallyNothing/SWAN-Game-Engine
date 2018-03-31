#ifndef SWAN_CAMERA_HPP
#define SWAN_CAMERA_HPP

#include "Core/Display.hpp"       // For Display::GetWidth(), Display::GetHeight()
#include "Maths/Vector.hpp"       // For vec2
#include "Physics/Transform.hpp"  // For Transform
#include "Utility/AngleUnits.hpp" // For SWAN::Util::Radians

namespace SWAN {
struct OrthographicT {};

class Camera {
  public:
	Camera(float aspect      = Display::GetAspectRatio(),
	       vec3 pos          = vec3(0.0f),
	       Util::Radians fov = SWAN::Util::Radians::FromDegrees(90.0f),
	       float zNear = 0.1f, float zFar = 100.0f)
	    : aspect(aspect), fov(fov), zNear(zNear), zFar(zFar), transform(pos) {}

	explicit Camera(OrthographicT)
	    : aspect(0), fov(0), zNear(0), zFar(100), ortho(true),
	      transform(vec3(0.0f)),
	      perspective(
					  OrthoProject(0, Display::GetWidth(), Display::GetHeight(), 0, zNear, zFar)) {}

	inline vec3 getForw() const { return transform.getForw(); }
	inline vec3 getUp() const { return transform.getUp(); }

	inline mat4 getView() const {
		return LookAt(transform.pos,
					  transform.pos + transform.getForw(),
		              transform.getUp());
	}
	inline mat4 getPerspective() const {
		return ortho ? perspective : Perspective(fov, aspect, zNear, zFar);
	}

	void moveRight(float amt) { transform.pos += transform.getRight() * amt; }
	void moveForw(float amt) { transform.pos += transform.getForw() * amt; }
	void moveUp(float amt) { transform.pos += transform.getUp() * amt; }

	//----------------------------TRANSLATION----------------------------//
	inline void moveByX(float amt) { transform.pos.x += amt; }
	inline void moveByY(float amt) { transform.pos.y += amt; }
	inline void moveByZ(float amt) { transform.pos.z += amt; }
	inline void moveByVec(vec3 vec) { transform.pos += vec; }
	//----------------------------ROTATION----------------------------//
	inline void rotateByX(Util::Radians amt) {
		transform.rot.x += (double) amt;
	}
	inline void rotateByY(Util::Radians amt) {
		transform.rot.y += (double) amt;
	}
	inline void rotateByZ(Util::Radians amt) {
		transform.rot.z += (double) amt;
	}

	float aspect, fov, zNear, zFar;
	Transform transform;

  private:
	mat4 perspective;
	bool ortho = false;
};
} // namespace SWAN
#endif
