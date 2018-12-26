#include "Shader.hpp"

#include "../Core/Logging.hpp"

#include "Utility/Debug.hpp"
#include "Utility/Group.hpp"

#include <fstream>
#include <iostream>
#include <vector>

namespace SWAN {
    SWAN_DEBUG_VAR(std::string, dbg_vertexShader);
    SWAN_DEBUG_VAR(std::string, dbg_fragmentShader);

    void Shader::compileShaders(const std::string& vertexShaderFilepath,
				const std::string& fragmentShaderFilepath) {
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if(!vertexShaderID) std::cerr << "Vertex shader failed to be created!" << std::endl;
	if(!fragmentShaderID) std::cerr << "Fragment shader failed to be created!" << std::endl;
	compileShader(vertexShaderFilepath, vertexShaderID);
	compileShader(fragmentShaderFilepath, fragmentShaderID);

	SWAN_DEBUG_DO(dbg_vertexShader = vertexShaderFilepath);
	SWAN_DEBUG_DO(dbg_fragmentShader = fragmentShaderFilepath);
    }

    void Shader::linkShaders() {
	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int*) &isLinked);
	if(isLinked == GL_FALSE) {
	    GLint maxLength = 0;
	    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

	    std::vector<char> errorLog(maxLength);
	    glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

	    glDeleteProgram(programID);
	    // Don't leak shaders either.
	    glDeleteShader(vertexShaderID);
	    glDeleteShader(fragmentShaderID);

	    // print the error log and quit
	    std::printf("%s\n", &(errorLog[0]));
	    std::cerr << "Shaders failed to link!" << std::endl;
	    SWAN_DEBUG_OUT(dbg_vertexShader);
	    SWAN_DEBUG_OUT(dbg_fragmentShader);
	}

	// Always detach shaders after a successful link.
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
    }

    void Shader::addAttrib(const std::string& attributeName) {
	glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
    }

    void Shader::use() {
	if(!used) {
	    glUseProgram(programID);
	    for(int i = 0; i < numAttributes; i++) glEnableVertexAttribArray(i);
	    used = true;
	}
    }

    // disable the shader
    void Shader::unuse() {
	if(used) {
	    glUseProgram(0);
	    for(int i = 0; i < numAttributes; i++) glDisableVertexAttribArray(i);
	    used = false;
	}
    }

    void Shader::compileShadersFromSrc(const char *vertSrc, const char *fragSrc) {
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	comp(vertSrc, vertexShaderID);
	comp(fragSrc, fragmentShaderID);
    }

    void Shader::comp(const char* src, GLuint id) {
	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(success == GL_FALSE) {
	    GLint maxLength = 0;
	    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

	    std::vector<char> errorLog(maxLength);
	    glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());

	    glDeleteShader(id); 

	    std::printf("%s\n", errorLog.data());
	    std::cerr << "Shader " << id << " failed to compile" << std::endl;
	}
    }

    // Compiles a single shader file
    void Shader::compileShader(const std::string& filePath, GLuint id) {
	// Open the file
	std::ifstream shaderFile(filePath);
	if(shaderFile.fail()) {
	    perror(filePath.c_str());
	    std::cerr << "Failed to open " + filePath << std::endl;
	}

	// File contents stores all the text in the file
	std::string fileContents = "";
	// line is used to grab each line of the file
	std::string line;

	// Get all the lines in the file and add it to the contents
	while(std::getline(shaderFile, line))
	    fileContents += line + "\n";

	shaderFile.close();
	comp(fileContents.c_str(), id);
    }

    bool Shader::hasUniform(const std::string& name) {
#ifdef SWAN_DEBUG_SHADER
	if(uniforms.find(name) == uniforms.end()) {
	    Log("Shader",
		std::string("Attempting to set uniform variable \"") + name + "\", which doesn't seem to exist, try adding it first.",
		LogLevel::Warning
	    );
	    return false;
	}
	return true;
#else
	return uniforms.find(name) != uniforms.end();
#endif
    }

    void Shader::addUniform(const std::string& name) {
	auto res = glGetUniformLocation(programID, name.c_str());
	if(res >= 0) {
	    uniforms.insert({ name, res });
	} else {
	    Log("Shader",
		std::string("Attempted to add uniform \"") + name + "\", but the uniform doesn't exist.",
		LogLevel::Error
	    );
	}
    }

    GLint Shader::getUniformID(const std::string& name) { return uniforms[name]; }

    void Shader::setUniformData(const std::string& name, int data) {
	if(hasUniform(name))
	    glUniform1i(uniforms[name], data);
    }

    void Shader::setUniformData(const std::string& name, float data) {
	if(hasUniform(name))
	    glUniform1f(uniforms[name], data);
    }

    void Shader::setUniformData(const std::string& name, double data) {
	if(hasUniform(name))
	    glUniform1f(uniforms[name], data);
    }

    void Shader::setUniformData(const std::string& name, vec2 data) {
	if(hasUniform(name))
	    glUniform2f(uniforms[name], data.x, data.y);
    }

    void Shader::setUniformData(const std::string& name, vec3 data) {
	if(hasUniform(name))
	    glUniform3f(uniforms[name], data.x, data.y, data.z);
    }

    void Shader::setUniformData(const std::string& name, vec4 data) {
	if(hasUniform(name))
	    glUniform4f(uniforms[name], data.x, data.y, data.z, data.w);
    }

    void Shader::setUniformData(const std::string& name, const mat2& data,
				GLboolean transposed) {
	if(hasUniform(name))
	    glUniformMatrix2fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
    }

    void Shader::setUniformData(const std::string& name, const mat3& data,
				GLboolean transposed) {
	if(hasUniform(name))
	    glUniformMatrix3fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
    }

    void Shader::setUniformData(const std::string& name, const mat4& data,
				GLboolean transposed) {
	if(hasUniform(name))
	    glUniformMatrix4fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
    }

    void Shader::setUniformData(const std::string& name, Transform& data) {
	if(hasUniform(name)) {
	    mat4 buff = data.getModel();
	    setUniformData(name, buff);
	}
    }

    void Shader::setUniformData(const std::string& name, bool data) {
	if(hasUniform(name))
	    setUniformData(name, data ? 1 : 0);
    }

    void Shader::setUniformData(const std::string& name, mat2&& data, GLboolean transposed) {
	if(hasUniform(name)) {
	    mat2 _m = data;
	    setUniformData(name, _m, transposed);
	}
    }

    void Shader::setUniformData(const std::string& name, mat3&& data, GLboolean transposed) {
	if(hasUniform(name)) {
	    mat3 _m = data;
	    setUniformData(name, _m, transposed);
	}
    }

    void Shader::setUniformData(const std::string& name, mat4&& data, GLboolean transposed) {
	if(hasUniform(name)) {
	    mat4 _m = data;
	    setUniformData(name, _m, transposed);
	}
    }

    void Shader::setUniformData(const std::string& name, DirectionalLight light) {
	setUniformData(name + ".dir", light.direction);

	setUniformData(name + ".type", L_DIRECTIONAL);

	setUniformData(name + ".ambient", light.ambient);
	setUniformData(name + ".diffuse", light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt", light.linearAtt);
	setUniformData(name + ".quadAtt", light.quadraticAtt);
    }

    void Shader::setUniformData(const std::string& name, PointLight light) {
	setUniformData(name + ".pos", light.position);

	setUniformData(name + ".type", L_POINT);

	setUniformData(name + ".ambient", light.ambient);
	setUniformData(name + ".diffuse", light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt", light.linearAtt);
	setUniformData(name + ".quadAtt", light.quadraticAtt);
    }

    void Shader::setUniformData(const std::string& name, Spotlight light) {
	setUniformData(name + ".pos", light.position);
	setUniformData(name + ".dir", light.direction);

	setUniformData(name + ".cutoff", (float) light.cutoff);
	setUniformData(name + ".outerCutoff", (float) light.outerCutoff);

	setUniformData(name + ".type", L_SPOT);

	setUniformData(name + ".ambient", light.ambient);
	setUniformData(name + ".diffuse", light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt", light.linearAtt);
	setUniformData(name + ".quadAtt", light.quadraticAtt);
    }

    void Shader::setUniform(ShaderUniform su) {
	if(!used && !lockUse)
	    use();

	const std::array<ShaderUniform::Type, 3> ComposedTypes = {
	    ShaderUniform::T_POINTLIGHT, ShaderUniform::T_SPOTLIGHT,
	    ShaderUniform::T_DIRLIGHT
	};

	if(!Util::IsOneOf(su.type, ComposedTypes.begin(), ComposedTypes.end()) &&
	   !hasUniform(su.name)) {
	    addUniform(su.name);
	    if(!hasUniform(su.name)) { // If the uniform STILL doesn't exist...
		// TODO: Log error/warning here
		return;
	    }
	}

	switch(su.type) {
	case ShaderUniform::T_INT: setUniformData(su.name, su.data.i); break;
	case ShaderUniform::T_BOOL: setUniformData(su.name, su.data.b); break;
	case ShaderUniform::T_FLOAT: setUniformData(su.name, su.data.f); break;
	case ShaderUniform::T_DOUBLE: setUniformData(su.name, su.data.d); break;

	case ShaderUniform::T_VEC2: setUniformData(su.name, su.data.v2); break;
	case ShaderUniform::T_VEC3: setUniformData(su.name, su.data.v3); break;
	case ShaderUniform::T_VEC4: setUniformData(su.name, su.data.v4); break;

	case ShaderUniform::T_MAT2: setUniformData(su.name, su.data.m2); break;
	case ShaderUniform::T_MAT3: setUniformData(su.name, su.data.m3); break;
	case ShaderUniform::T_MAT4: setUniformData(su.name, su.data.m4); break;

	case ShaderUniform::T_TEXTURE:
	    su.data.tex->bind();
	    break;

	case ShaderUniform::T_TRANSFORM:
	    setUniformData(su.name, su.data.transf);
	    break;

	case ShaderUniform::T_POINTLIGHT:
	    setUniformData(su.name, su.data.plight);
	    break;
	case ShaderUniform::T_DIRLIGHT:
	    setUniformData(su.name, su.data.dlight);
	    break;
	case ShaderUniform::T_SPOTLIGHT:
	    setUniformData(su.name, su.data.slight);
	    break;

	default: break;
	}

	if(!lockUse)
	    unuse();
    }
} // namespace SWAN
