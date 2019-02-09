#ifndef SWAN_LIGHT_HPP
#define SWAN_LIGHT_HPP

#include "Maths/Vector.hpp"
#include "Utility/AngleUnits.hpp"

namespace SWAN
{
	enum LightType {
		L_DIRECTIONAL = 0,
		L_POINT,
		L_SPOT
	};

	struct DirectionalLight {
		/// Direction the light source is pointing in.
		vec3 direction;

		/// Ambient color.
		vec3 ambient;
		/// Diffuse color.
		vec3 diffuse;
		/// Specular color.
		vec3 specular;

		/// Constant attenuation.
		float constAtt = 1.0;
		/// Linear attenuation.
		float linearAtt = 0.0;
		/// Quadratic attenuation.
		float quadraticAtt = 0.0;
	};

	struct PointLight {
		/// Position of the light source.
		vec3 position;

		/// Ambient color.
		vec3 ambient;
		/// Diffuse color.
		vec3 diffuse;
		/// Specular color.
		vec3 specular;

		/// Constant attenuation.
		float constAtt = 1.0;
		/// Linear attenuation.
		float linearAtt;
		/// Quadratic attenuation.
		float quadraticAtt;
	};

	struct Spotlight {
		/// Position of the light source.
		vec3 position;
		/// Direction the light source is pointing in.
		vec3 direction;

		Util::Radians cutoff;
		Util::Radians outerCutoff;

		/// Ambient color.
		vec3 ambient;
		/// Diffuse color.
		vec3 diffuse;
		/// Specular color.
		vec3 specular;

		/// Constant attenuation.
		float constAtt = 1.0;
		/// Linear attenuation.
		float linearAtt;
		/// Quadratic attenuation.
		float quadraticAtt;
	};

	/// A generic light source.
	struct Light {
		Light() {}
		explicit Light(LightType type) : type(type) {}

		explicit Light(DirectionalLight dl)
		{
			type = L_DIRECTIONAL;
			dirLight = dl;
		}
		explicit Light(PointLight pl)
		{
			type = L_POINT;
			pointLight = pl;
		}
		explicit Light(Spotlight sl)
		{
			type = L_SPOT;
			spotLight = sl;
		}

		Light& operator=(DirectionalLight dl)
		{
			type = L_DIRECTIONAL;
			dirLight = dl;
			return *this;
		}
		Light& operator=(PointLight pl)
		{
			type = L_POINT;
			pointLight = pl;
			return *this;
		}
		Light& operator=(Spotlight sl)
		{
			type = L_SPOT;
			spotLight = sl;
			return *this;
		}

		explicit operator DirectionalLight() { return dirLight; }
		explicit operator PointLight() { return pointLight; }
		explicit operator Spotlight() { return spotLight; }

		LightType type;
		union {
			DirectionalLight dirLight;
			PointLight pointLight;
			Spotlight spotLight;
		};
	};
} // namespace SWAN

#endif // LIGHT_HPP
