#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "Light.hpp"
#include "Mesh.hpp"
#include "Physics/Transform.hpp"
#include "Texture.hpp"

namespace SWAN {
// TODO: Rewrite Shader class

struct ShaderUniform {
	std::string name;

	enum Type {
		T_INT,
		T_BOOL,
		T_FLOAT,
		T_DOUBLE,

		T_VEC2,
		T_VEC3,
		T_VEC4,
		T_MAT2,
		T_MAT3,
		T_MAT4,

		T_TEXTURE,
		T_TRANSFORM,

		T_POINTLIGHT,
		T_DIRLIGHT,
		T_SPOTLIGHT,
	} type;

	union Data {
		Data() {}
		~Data() {}

		int i;
		bool b;
		float f;
		double d;

		glm::vec2 v2;
		glm::vec3 v3;
		glm::vec4 v4;

		glm::mat2 m2;
		glm::mat3 m3;
		glm::mat4 m4;

		const Texture* tex;

		Transform transf;

		PointLight plight;
		DirectionalLight dlight;
		Spotlight slight;
	} data;

	ShaderUniform(std::string name, int i) : name(name) {
		data.i = i;
		type   = T_INT;
	}
	ShaderUniform(std::string name, bool b) : name(name) {
		data.b = b;
		type   = T_BOOL;
	}
	ShaderUniform(std::string name, float f) : name(name) {
		data.f = f;
		type   = T_FLOAT;
	}

	ShaderUniform(std::string name, glm::vec2 v2) : name(name) {
		data.v2 = v2;
		type    = T_VEC2;
	}
	ShaderUniform(std::string name, glm::vec3 v3) : name(name) {
		data.v3 = v3;
		type    = T_VEC3;
	}
	ShaderUniform(std::string name, glm::vec4 v4) : name(name) {
		data.v4 = v4;
		type    = T_VEC4;
	}

	ShaderUniform(std::string name, glm::mat2 m2) : name(name) {
		data.m2 = m2;
		type    = T_MAT2;
	}
	ShaderUniform(std::string name, glm::mat3 m3) : name(name) {
		data.m3 = m3;
		type    = T_MAT3;
	}
	ShaderUniform(std::string name, glm::mat4 m4) : name(name) {
		data.m4 = m4;
		type    = T_MAT4;
	}

	ShaderUniform(std::string name, const Texture* tex) : name(name) {
		data.tex = tex;
		type     = T_TEXTURE;
	}

	ShaderUniform(std::string name, Transform transf) : name(name) {
		data.transf = transf;
		type        = T_TRANSFORM;
	}

	ShaderUniform(std::string name, PointLight plight) : name(name) {
		data.plight = plight;
		type        = T_POINTLIGHT;
	}
	ShaderUniform(std::string name, DirectionalLight dlight)
	    : name(name) {
		data.dlight = dlight;
		type        = T_DIRLIGHT;
	}
	ShaderUniform(std::string name, Spotlight slight) : name(name) {
		data.slight = slight;
		type        = T_SPOTLIGHT;
	}
	ShaderUniform(std::string name, Light light) : name(name) {
		switch(light.type) {
			case L_DIRECTIONAL:
				type        = T_DIRLIGHT;
				data.dlight = light.dirLight;
				break;
			case L_POINT:
				type        = T_POINTLIGHT;
				data.plight = light.pointLight;
				break;
			case L_SPOT:
				type        = T_SPOTLIGHT;
				data.slight = light.spotLight;
				break;
		}
	}

	ShaderUniform() {}

	ShaderUniform(const ShaderUniform& other)
	    : name(other.name), data(other.data), type(other.type) {}

	ShaderUniform(ShaderUniform&& other)
	    : name(std::move(other.name)), data(std::move(other.data)),
	      type(std::move(other.type)) {}

	ShaderUniform& operator=(const ShaderUniform& other) {
		name = other.name;
		data = other.data;
		type = other.type;

		return *this;
	}
};

//-----------------------------------------------------------------------------------------
// Note:
//     This class was taken pretty much verbatim from MakingGamesWithBen's
//     excellent "Advanced C++/Graphics Tutorials" series. You should check it out.
//     The only things that were added to it were all of the "setUniformData" functions.
// Link:
//     https://www.youtube.com/playlist?list=PLSPw4ASQYyymu3PfG9gxywSPghnSMiOAW
//-----------------------------------------------------------------------------------------
// This class handles the compilation, linking, and usage of a GLSL shader
// program.
// Reference: http://www.opengl.org/wiki/Shader_Compilation
//-----------------------------------------------------------------------------------------
class Shader {
  public:
	Shader();
	~Shader();

	void compileShaders(const std::string& vertexShaderFilePath,
	                    const std::string& fragmentShaderFilepath);

	void linkShaders();
	void addAttrib(const std::string& attributeName);

	void use();
	void unuse();

	GLuint getID() { return _programID; }

	bool hasUniform(const std::string& name);
	void addUniform(const std::string& name);

	GLint getUniformID(const std::string& name);

	void setUniform(ShaderUniform su);

	inline void setUniforms(std::vector<ShaderUniform> su) {
		lockUse = true;

		if(!used)
			use();
		for(auto s : su)
			setUniform(s);

		lockUse = false;
		unuse();
	}

	inline void renderBatch(const std::vector<const Mesh*> batch) {
		use();
		for(const auto* m : batch)
			m->render();
		unuse();
	}

	inline void renderMesh(const Mesh& m) {
		use();
		m.render();
		unuse();
	}

	inline void renderWireframeMesh(const Mesh& m) {
		use();
		m.renderWireframe();
		unuse();
	}

	inline void renderMeshVerts(const Mesh& m) {
		use();
		m.renderVerts();
		unuse();
	}

	void setUniformData(const std::string& name, int data);
	void setUniformData(const std::string& name, bool data);
	void setUniformData(const std::string& name, float data);
	void setUniformData(const std::string& name, double data);

	void setUniformData(const std::string& name, glm::vec2 data);
	void setUniformData(const std::string& name, glm::vec3 data);
	void setUniformData(const std::string& name, glm::vec4 data);

	void setUniformData(const std::string& name, glm::mat2& data,
	                    GLboolean transposed = GL_FALSE);
	void setUniformData(const std::string& name, glm::mat3& data,
	                    GLboolean transposed = GL_FALSE);
	void setUniformData(const std::string& name, glm::mat4& data,
	                    GLboolean transposed = GL_FALSE);

	void setUniformData(const std::string& name, glm::mat2&& data,
	                    GLboolean transposed = GL_FALSE);
	void setUniformData(const std::string& name, glm::mat3&& data,
	                    GLboolean transposed = GL_FALSE);
	void setUniformData(const std::string& name, glm::mat4&& data,
	                    GLboolean transposed = GL_FALSE);

	void setUniformData(const std::string& name, Transform& data);

	void setUniformData(const std::string& name, DirectionalLight light);
	void setUniformData(const std::string& name, PointLight light);
	void setUniformData(const std::string& name, Spotlight light);
	void setUniformData(const std::string& name, Light light);

  private:
	bool used    = false;
	bool lockUse = false;

	int _numAttributes;

	void compileShader(const std::string& filePath, GLuint id);

	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;

	std::map<std::string, GLint> uniforms;
};
} // namespace SWAN
