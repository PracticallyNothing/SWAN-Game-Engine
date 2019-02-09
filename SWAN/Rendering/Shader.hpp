#pragma once

#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "Core/Defs.hpp"
#include "Light.hpp"
#include "Maths/Vector.hpp"
#include "Mesh.hpp"
#include "Physics/Transform.hpp"
#include "Texture.hpp"

namespace SWAN
{
	/// A representation of an OpenGL shader uniform.
	struct ShaderUniform {
		String Name;

		/// Type of shader data.
		enum class Type {
			Boolean,
			Integer,
			Unsigned,
			Float,
			Double,
			Vector2,
			Vector3,
			Vector4,
			Matrix2x2,
			Matrix3x3,
			Matrix4x4,
			Transform,
			Sampler2D,
			PointLight,
			DirLight,
			Spotlight,
		};

		enum Type Type;

		/// Union with data for uniform.
		union Data {
			Data() {}
			~Data() {}

			bool BooleanData;
			int IntegerData;
			unsigned UnsignedData;
			float FloatData;
			double DoubleData;

			vec2 Vector2Data;
			vec3 Vector3Data;
			vec4 Vector4Data;

			mat2 Matrix2x2Data;
			mat3 Matrix3x3Data;
			mat4 Matrix4x4Data;

			const Texture* Sampler2DData;

			Transform TransformData;

			PointLight PointLightData;
			DirectionalLight DirLightData;
			Spotlight SpotlightData;
		};
		union Data Data;

		ShaderUniform(const String& name, int i) : Name(name)
		{
			Data.IntegerData = i;
			Type = Type::Integer;
		}
		ShaderUniform(const String& name, bool b) : Name(name)
		{
			Data.BooleanData = b;
			Type = Type::Boolean;
		}
		ShaderUniform(const String& name, float f) : Name(name)
		{
			Data.FloatData = f;
			Type = Type::Float;
		}

		ShaderUniform(const String& name, vec2 v2) : Name(name)
		{
			Data.Vector2Data = v2;
			Type = Type::Vector2;
		}
		ShaderUniform(const String& name, vec3 v3) : Name(name)
		{
			Data.Vector3Data = v3;
			Type = Type::Vector3;
		}
		ShaderUniform(const String& name, vec4 v4) : Name(name)
		{
			Data.Vector4Data = v4;
			Type = Type::Vector4;
		}

		ShaderUniform(const String& name, mat2 m2) : Name(name)
		{
			Data.Matrix2x2Data = m2;
			Type = Type::Matrix2x2;
		}
		ShaderUniform(const String& name, mat3 m3) : Name(name)
		{
			Data.Matrix3x3Data = m3;
			Type = Type::Matrix3x3;
		}
		ShaderUniform(const String& name, mat4 m4) : Name(name)
		{
			Data.Matrix4x4Data = m4;
			Type = Type::Matrix4x4;
		}

		ShaderUniform(const String& name, const Texture* tex) : Name(name)
		{
			Data.Sampler2DData = tex;
			Type = Type::Sampler2D;
		}
		ShaderUniform(const String& name, Transform transf) : Name(name)
		{
			Data.TransformData = transf;
			Type = Type::Transform;
		}

		ShaderUniform(const String& name, Spotlight slight) : Name(name)
		{
			Data.SpotlightData = slight;
			Type = Type::Spotlight;
		}
		ShaderUniform(const String& name, PointLight plight) : Name(name)
		{
			Data.PointLightData = plight;
			Type = Type::PointLight;
		}
		ShaderUniform(const String& name, DirectionalLight dlight) : Name(name)
		{
			Data.DirLightData = dlight;
			Type = Type::DirLight;
		}

		/** Construct a uniform from a Light. */
		ShaderUniform(const String& name, Light light) : Name(name)
		{
			switch(light.type) {
				case L_DIRECTIONAL: *this = light.dirLight; break;
				case L_POINT: *this = light.pointLight; break;
				case L_SPOT: *this = light.spotLight; break;
			}
		}

		ShaderUniform(const String& name)
		    : Name(name) {}

		ShaderUniform& operator=(int i)
		{
			Data.IntegerData = i;
			Type = Type::Integer;
			return *this;
		}
		ShaderUniform& operator=(bool b)
		{
			Data.BooleanData = b;
			Type = Type::Boolean;
			return *this;
		}
		ShaderUniform& operator=(float f)
		{
			Data.FloatData = f;
			Type = Type::Float;
			return *this;
		}
		ShaderUniform& operator=(vec2 v2)
		{
			Data.Vector2Data = v2;
			Type = Type::Vector2;
			return *this;
		}
		ShaderUniform& operator=(vec3 v3)
		{
			Data.Vector3Data = v3;
			Type = Type::Vector3;
			return *this;
		}
		ShaderUniform& operator=(vec4 v4)
		{
			Data.Vector4Data = v4;
			Type = Type::Vector4;
			return *this;
		}
		ShaderUniform& operator=(mat2 m2)
		{
			Data.Matrix2x2Data = m2;
			Type = Type::Matrix2x2;
			return *this;
		}
		ShaderUniform& operator=(mat3 m3)
		{
			Data.Matrix3x3Data = m3;
			Type = Type::Matrix3x3;
			return *this;
		}
		ShaderUniform& operator=(mat4 m4)
		{
			Data.Matrix4x4Data = m4;
			Type = Type::Matrix4x4;
			return *this;
		}
		ShaderUniform& operator=(const Texture* tex)
		{
			Data.Sampler2DData = tex;
			Type = Type::Sampler2D;
			return *this;
		}
		ShaderUniform& operator=(Transform transf)
		{
			Data.TransformData = transf;
			Type = Type::Transform;
			return *this;
		}
		ShaderUniform& operator=(Spotlight slight)
		{
			Data.SpotlightData = slight;
			Type = Type::Spotlight;
			return *this;
		}
		ShaderUniform& operator=(PointLight plight)
		{
			Data.PointLightData = plight;
			Type = Type::PointLight;
			return *this;
		}
		ShaderUniform& operator=(DirectionalLight dlight)
		{
			Data.DirLightData = dlight;
			Type = Type::DirLight;
			return *this;
		}

		/** Construct a uniform from a Light. */
		ShaderUniform& operator=(Light light)
		{
			switch(light.type) {
				case L_DIRECTIONAL: return *this = light.dirLight;
				case L_POINT: return *this = light.pointLight;
				case L_SPOT: return *this = light.spotLight;
			}
			return *this;
		}

		/// Default constructor.
		ShaderUniform() {}
		/// Copy constructor.
		ShaderUniform(const ShaderUniform& other) : Name(other.Name), Data(other.Data), Type(other.Type) {}
		/// Move constructor.
		ShaderUniform(ShaderUniform&& other) : Name(std::move(other.Name)), Data(std::move(other.Data)), Type(std::move(other.Type)) {}

		/// Copy assignment operator.
		ShaderUniform& operator=(const ShaderUniform& other)
		{
			Name = other.Name;
			Data = other.Data;
			Type = other.Type;

			return *this;
		}

		/// Move assignment operator.
		ShaderUniform& operator=(ShaderUniform&& other)
		{
			Name = std::move(other.Name);
			Data = std::move(other.Data);
			Type = std::move(other.Type);
			return *this;
		}
	};

	//-----------------------------------------------------------------------------------------
	// Note:
	//     This class was taken pretty much verbatim from MakingGamesWithBen's
	//     excellent "Advanced C++/Graphics Tutorials" series. You should check it out.
	//     The only things that were added to it were the "Set[Something]()" functions.
	// Link:
	//     https://www.youtube.com/playlist?list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW
	//-----------------------------------------------------------------------------------------
	// This class handles the compilation, linking, and usage of a GLSL shader program.
	// Reference: http://www.opengl.org/wiki/Shader_Compilation
	//-----------------------------------------------------------------------------------------

	/// Representation of an OpenGL shader.
	class Shader
	{
	  public:
		/// Read the contents of the vertex and fragment shader files,
		void compileShaders(const std::string& vertexShaderFilepath,
		                    const std::string& fragmentShaderFilepath);

		void compileShadersFromSrc(const char* vertSrc, const char* fragSrc);

		void linkShaders();
		void addAttrib(const std::string& attributeName);

		void use();
		void unuse();

		GLuint getID() { return programID; }

		/** @brief Does this shader have this uniform?
	 *  
	 *  @note Shaders don't autodetect uniforms (yet). 
	 *        This checks if a call to addUniform() with the same name has succeeded.
	 *
	 *  @param name The name of the uniform.
	 *  @return Whether the uniform is in the shader.
	 */
		bool hasUniform(const std::string& name);
		void addUniform(const std::string& name);

		GLint getUniformID(const std::string& name);

		void SetInt(const String& name, int value);
		void SetReal(const String& name, double value);

		void SetVec2(const String& name, vec2 vec);
		void SetVec3(const String& name, vec3 vec);
		void SetVec4(const String& name, vec4 vec);

		void SetMat2(const String& name, mat2 mat, GLboolean transposed = GL_TRUE);
		void SetMat3(const String& name, mat3 mat, GLboolean transposed = GL_TRUE);
		void SetMat4(const String& name, mat4 mat, GLboolean transposed = GL_TRUE);

	  private:
		int numAttributes = 0;

		void compileShader(const std::string& filePath, GLuint id);
		void comp(const char* src, GLuint id);

		GLuint programID = 0;
		GLuint vertexShaderID = 0;
		GLuint fragmentShaderID = 0;

		std::map<std::string, GLint> uniforms;
	};
} // namespace SWAN
