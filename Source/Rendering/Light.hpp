#ifndef SWAN_LIGHT_HPP
#define SWAN_LIGHT_HPP

#include "Utility/AngleUnits.hpp"
#include <glm/glm.hpp>
namespace SWAN {
enum LightType {
	L_DIRECTIONAL = 0,
	L_POINT,
	L_SPOT
};

struct DirectionalLight {
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constAtt     = 1.0; // Constant attenuation
	float linearAtt    = 0.0; // Linear attenuation
	float quadraticAtt = 0.0; // Quadratic attenuation
};

struct PointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constAtt = 1.0; // Constant attenuation
	float linearAtt;      // Linear attenuation
	float quadraticAtt;   // Quadratic attenuation
};

struct Spotlight {
	glm::vec3 position;
	glm::vec3 direction;

	Util::Radians cutoff;
	Util::Radians outerCutoff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constAtt = 1.0; // Constant attenuation
	float linearAtt;      // Linear attenuation
	float quadraticAtt;   // Quadratic attenuation
};
}

#endif //LIGHT_HPP
