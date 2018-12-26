#pragma once

#include <map>
#include <string>
#include <vector>
#include <initializer_list>

#include <glad/glad.h>

#include "Light.hpp"
#include "Maths/Vector.hpp"
#include "Mesh.hpp"
#include "Physics/Transform.hpp"
#include "Texture.hpp"

namespace SWAN {
    /// A representation of an OpenGL shader uniform.
    struct ShaderUniform {
	std::string name;

	/// Type of shader data.
	enum Type {
	    /// Integer uniform
	    T_INT,
	    /// Boolean uniform
	    T_BOOL,
	    /// Float uniform
	    T_FLOAT,
	    /// Double uniform
	    T_DOUBLE,

	    /// 2D vector uniform
	    T_VEC2,
	    /// 3D vector uniform
	    T_VEC3,
	    /// 4D vector uniform
	    T_VEC4,

	    /// 2x2 matrix uniform
	    T_MAT2,
	    /// 3x3 matrix uniform
	    T_MAT3,
	    /// 4x4 matrix uniform
	    T_MAT4,

	    /// Texture uniform
	    /// @note This is not handled the correct way. This just binds the given texture.
	    T_TEXTURE,

	    /// Transform uniform
	    /// @note This uniform is just transformed to a mat4.
	    T_TRANSFORM,

	    /// Point light uniform
	    T_POINTLIGHT,
	    /// Directional light uniform
	    T_DIRLIGHT,
	    /// Spotlight uniform
	    T_SPOTLIGHT,
	} type;

	/// Union with data for uniform.
	union Data {
	    Data() {}
	    ~Data() {}

	    /// Integer data
	    int i;
	    /// Boolean data
	    bool b;
	    /// Float data
	    float f;
	    /// Double data
	    double d;

	    /// 2D vector data
	    vec2 v2;
	    /// 3D vector data
	    vec3 v3;
	    /// 4D vector data
	    vec4 v4;

	    /// 2x2 matrix data
	    mat2 m2;
	    /// 3x3 matrix data
	    mat3 m3;
	    /// 4x4 matrix data
	    mat4 m4;

	    /// Texture data
	    /// @note This is not handled the correct way. This just binds the given texture.
	    const Texture* tex;

	    /// Transform data
	    /// @note This data is just transformed to a mat4.
	    Transform transf;

	    /// Point light data
	    PointLight plight;
	    /// Directional light data
	    DirectionalLight dlight;
	    /// Spotlight data
	    Spotlight slight;

	} data;

	/** Construct a uniform from a int. */
	ShaderUniform(std::string name, int i) : name(name) { data.i = i; type = T_INT; }
	/** Construct a uniform from a bool. */
	ShaderUniform(std::string name, bool b) : name(name) { data.b = b; type = T_BOOL; }
	/** Construct a uniform from a float. */
	ShaderUniform(std::string name, float f) : name(name) { data.f = f; type = T_FLOAT; }

	/** Construct a uniform from a vec2. */
	ShaderUniform(std::string name, vec2 v2) : name(name) { data.v2 = v2; type = T_VEC2; }
	/** Construct a uniform from a vec3. */
	ShaderUniform(std::string name, vec3 v3) : name(name) { data.v3 = v3; type = T_VEC3; }
	/** Construct a uniform from a vec4. */
	ShaderUniform(std::string name, vec4 v4) : name(name) { data.v4 = v4; type = T_VEC4; }

	/** Construct a uniform from a mat2. */
	ShaderUniform(std::string name, mat2 m2) : name(name) { data.m2 = m2; type = T_MAT2; }
	/** Construct a uniform from a mat3. */
	ShaderUniform(std::string name, mat3 m3) : name(name) { data.m3 = m3; type = T_MAT3; }
	/** Construct a uniform from a mat4. */
	ShaderUniform(std::string name, mat4 m4) : name(name) { data.m4 = m4; type = T_MAT4; }

	/** Construct a uniform from a Texture. */
	ShaderUniform(std::string name, const Texture* tex) : name(name) { data.tex = tex; type = T_TEXTURE; }
	/** Construct a uniform from a Transform. */
	ShaderUniform(std::string name, Transform transf) : name(name) { data.transf = transf; type = T_TRANSFORM; }

	/** Construct a uniform from a PointLight. */
	ShaderUniform(std::string name, PointLight plight) : name(name) { data.plight = plight; type = T_POINTLIGHT; }
	/** Construct a uniform from a DirectionalLight. */
	ShaderUniform(std::string name, DirectionalLight dlight) : name(name) { data.dlight = dlight; type = T_DIRLIGHT; }
	/** Construct a uniform from a Spotlight. */
	ShaderUniform(std::string name, Spotlight slight) : name(name) { data.slight = slight; type = T_SPOTLIGHT; }

	/** Construct a uniform from a Light. */
	ShaderUniform(std::string name, Light light) : name(name) {
	    switch(light.type) {
	    case L_DIRECTIONAL:
		type = T_DIRLIGHT;
		data.dlight = light.dirLight;
		break;
	    case L_POINT:
		type = T_POINTLIGHT;
		data.plight = light.pointLight;
		break;
	    case L_SPOT:
		type = T_SPOTLIGHT;
		data.slight = light.spotLight;
		break;
	    }
	}

	/// Default constructor.
	ShaderUniform() {}
	/// Copy constructor.
	ShaderUniform(const ShaderUniform& other) : name(other.name), data(other.data), type(other.type) {}
	/// Move constructor.
	ShaderUniform(ShaderUniform&& other) : name(std::move(other.name)), data(std::move(other.data)), type(std::move(other.type)) {}

	/// Copy assignment operator.
	ShaderUniform& operator=(const ShaderUniform& other) {
	    name = other.name;
	    data = other.data;
	    type = other.type;

	    return *this;
	}

	/// Move assignment operator.
	ShaderUniform& operator=(ShaderUniform&& other) {
	    name = std::move(other.name);
	    data = std::move(other.data);
	    type = std::move(other.type);
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
    /// Representation of an OpenGL shader.
    class Shader {
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
	/** @brief 
	 *
	 */
	void addUniform(const std::string& name);

	GLint getUniformID(const std::string& name);

	void setUniform(ShaderUniform su);

	inline void setUniforms(std::vector<ShaderUniform> su) {
	    lockUse = true;
	    if(!used) use();
	    for(auto s : su) setUniform(s);
	    lockUse = false;
	    unuse();
	}

	inline void setUniforms(std::initializer_list<ShaderUniform> su) {
	    lockUse = true;
	    if(!used) use();
	    for(auto s : su) setUniform(s);
	    lockUse = false;
	    unuse();
	}

	inline void renderBatch(const std::vector<const Mesh*> batch) {
	    use();
	    for(const auto* m : batch) m->render();
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

	void setUniformData(const std::string& name, vec2 data);
	void setUniformData(const std::string& name, vec3 data);
	void setUniformData(const std::string& name, vec4 data);

	void setUniformData(const std::string& name, const mat2& data, GLboolean transposed = GL_TRUE);
	void setUniformData(const std::string& name, const mat3& data, GLboolean transposed = GL_TRUE);
	void setUniformData(const std::string& name, const mat4& data, GLboolean transposed = GL_TRUE);

	void setUniformData(const std::string& name, mat2&& data, GLboolean transposed = GL_TRUE);
	void setUniformData(const std::string& name, mat3&& data, GLboolean transposed = GL_TRUE);
	void setUniformData(const std::string& name, mat4&& data, GLboolean transposed = GL_TRUE);

	void setUniformData(const std::string& name, Transform& data);

	void setUniformData(const std::string& name, DirectionalLight light);
	void setUniformData(const std::string& name, PointLight light);
	void setUniformData(const std::string& name, Spotlight light);
	void setUniformData(const std::string& name, Light light);

    private:
	bool used    = false;
	bool lockUse = false;

	int numAttributes = 0;

	void compileShader(const std::string& filePath, GLuint id);
	void comp(const char* src, GLuint id);

	GLuint programID = 0;
	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;

	std::map<std::string, GLint> uniforms;
    };
} // namespace SWAN
