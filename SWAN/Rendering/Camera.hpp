#ifndef SWAN_CAMERA_HPP
#define SWAN_CAMERA_HPP

#include "Core/Display.hpp"       // For Display::GetWidth(), Display::GetHeight()
#include "Maths/Vector.hpp"       // For vec2
#include "Physics/Transform.hpp"  // For Transform
#include "Utility/AngleUnits.hpp" // For SWAN::Util::Radians

namespace SWAN {
    /// Empty structure. Pass to Camera constructor for Orthographic camera.
    struct OrthographicT {};

    class Camera {
    public:
	/// Construct a camera with a perspective view.
	Camera(float aspect      = Display::GetAspectRatio(),
	       vec3 pos          = vec3(0.0f),
	       Util::Radians fov = SWAN::Util::Radians::FromDegrees(90.0f),
	       float zNear = 0.1, float zFar = 1000)
	    : aspect(aspect), fov(fov), zNear(zNear), zFar(zFar), transform(pos) {}

	/// Construct a camera in an orthographic view mode.
	explicit Camera(OrthographicT)
	    : aspect(0), fov(0), zNear(-1000), zFar(1000), ortho(true),
	      transform(vec3(0.0f)) {}

	/// Get the position of the camera.
	inline vec3 pos() const { return transform.pos; }

	/// Get the forward direction for the camera.
	inline vec3 forw() const { return transform.getForw(); }

	/// Get the upward direction for the camera.
	inline vec3 up() const { return transform.getUp(); }

	/// Get the right direction for the camera.
	inline vec3 right() const { return transform.getRight(); }

	/// Calculate the view matrix for the camera.
	inline mat4 getView() const { return LookAt(pos(), pos() + forw(), up()); }

	/// Get the perspective matrix for the camera.
	inline mat4 getPerspective() const {
	    return ortho
		? OrthoProject(0, Display::GetWidth(), Display::GetHeight(), 0, zNear, zFar)
		: Perspective(fov, aspect, zNear, zFar);
	}

	/// Move the camera to the right.
	void moveRight(float amt) { transform.pos += right() * amt; }

	/// Move the camera forward.
	void moveForw(float amt) { transform.pos += forw() * amt; }

	/// Move the camera up.
	void moveUp(float amt) { transform.pos += up() * amt; }

	//----------------------------TRANSLATION----------------------------//
	inline void moveByX(float amt) { transform.pos.x += amt; }
	inline void moveByY(float amt) { transform.pos.y += amt; }
	inline void moveByZ(float amt) { transform.pos.z += amt; }
	inline void moveByVec(vec3 vec) { transform.pos += vec; }
	//----------------------------ROTATION----------------------------//
	inline void rotateByX(Util::Radians amt) { transform.rot.x += (double) amt; }
	inline void rotateByY(Util::Radians amt) { transform.rot.y += (double) amt; }
	inline void rotateByZ(Util::Radians amt) { transform.rot.z += (double) amt; }

	float aspect, fov, zNear, zFar;
	Transform transform;

    private:
	bool ortho = false;
    };
} // namespace SWAN
#endif
