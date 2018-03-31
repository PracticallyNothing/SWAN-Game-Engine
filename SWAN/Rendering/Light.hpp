#ifndef SWAN_LIGHT_HPP
#define SWAN_LIGHT_HPP

#include "Utility/AngleUnits.hpp"
#include "Maths/Vector.hpp"

namespace SWAN {
enum LightType { L_DIRECTIONAL = 0,
	             L_POINT,
	             L_SPOT };

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constAtt     = 1.0; // Constant attenuation
	float linearAtt    = 0.0; // Linear attenuation
	float quadraticAtt = 0.0; // Quadratic attenuation
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constAtt = 1.0; // Constant attenuation
	float linearAtt;      // Linear attenuation
	float quadraticAtt;   // Quadratic attenuation
};

struct Spotlight {
	vec3 position;
	vec3 direction;

	Util::Radians cutoff;
	Util::Radians outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constAtt = 1.0; // Constant attenuation
	float linearAtt;      // Linear attenuation
	float quadraticAtt;   // Quadratic attenuation
};

struct Light {
	Light() {}
	Light(LightType type) : type(type) {}

	explicit Light(DirectionalLight dl) {type = L_DIRECTIONAL; dirLight = dl;}
	explicit Light(PointLight pl) {type = L_POINT; pointLight = pl;}
	explicit Light(Spotlight sl) {type = L_SPOT; spotLight = sl;}

	explicit operator DirectionalLight() {return dirLight;}
	explicit operator PointLight() {return pointLight;}
	explicit operator Spotlight() {return spotLight;}

	LightType type;
	union {
		DirectionalLight dirLight;
		PointLight pointLight;
		Spotlight spotLight;
	};

};
} // namespace SWAN

#endif // LIGHT_HPP
