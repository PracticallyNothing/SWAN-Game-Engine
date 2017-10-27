#ifndef SWAN_CAMERA_HPP
#define SWAN_CAMERA_HPP

#include "Core/Display.hpp"       // For Display::GetWidth(), Display::GetHeight()
#include "Physics/Transform.hpp"  // For Transform
#include "Utility/AngleUnits.hpp" // For SWAN::Util::Radians
#include <glm/glm.hpp>            // For glm::vec3, glm::radians

namespace SWAN {
struct OrthographicT {};

class Camera {
  public:
	Camera (
	  float         aspect = Display::GetAspectRatio (),
	  glm::vec3     pos    = glm::vec3 (0.0f),
	  Util::Radians fov    = SWAN::Util::Radians::FromDegrees (90.0f),
	  float         zNear  = 0.1f,
	  float         zFar   = 100.0f)
	  : aspect (aspect)
	  , fov (fov)
	  , zNear (zNear)
	  , zFar (zFar)
	  , transform (pos) {}

	explicit Camera (OrthographicT)
	  : aspect (0)
	  , fov (0)
	  , zNear (0)
	  , zFar (0)
	  , ortho (true)
	  , transform (glm::vec3 (0.0f))
	  , perspective (glm::ortho (0, Display::GetWidth (), Display::GetHeight (), 0)) {}

	inline glm::vec3 getForw () const { return transform.getForw (); }
	inline glm::vec3 getUp () const { return transform.getUp (); }

	inline Transform  getTransform () const { return transform; }
	inline Transform& getTransform_ref () { return transform; }

	inline glm::vec3 getPos () const { return transform.pos; }
	inline glm::vec3 getRot () const { return transform.rot; }

	inline glm::mat4 getView () const {
		return glm::lookAt (transform.pos,
		                    transform.pos + transform.getForw (),
		                    transform.getUp ());
	}
	inline glm::mat4 getPerspective () const { return ortho ? perspective : glm::perspective (fov, aspect, zNear, zFar); }

	void moveRight (float amt) { transform.pos += transform.getRight () * amt; }
	void moveForw (float amt) { transform.pos += transform.getForw () * amt; }
	void moveUp (float amt) { transform.pos += transform.getUp () * amt; }

	//----------------------------TRANSLATION----------------------------//
	inline void moveByX (float amt) { transform.pos.x += amt; }
	inline void moveByY (float amt) { transform.pos.y += amt; }
	inline void moveByZ (float amt) { transform.pos.z += amt; }
	inline void moveByVec (glm::vec3 vec) { transform.pos += vec; }
	//----------------------------ROTATION----------------------------//
	inline void rotateByX (Util::Radians amt) { transform.rot.x += (double) amt; }
	inline void rotateByY (Util::Radians amt) { transform.rot.y += (double) amt; }
	inline void rotateByZ (Util::Radians amt) { transform.rot.z += (double) amt; }

	float aspect, fov, zNear, zFar;

  private:
	Transform transform;
	glm::mat4 perspective;
	bool      ortho = false;
};
}
#endif
