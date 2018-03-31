#include "Shader.hpp"

#include "Utility/Debug.hpp"
#include "Utility/Group.hpp"

#include <fstream>
#include <iostream>
#include <vector>

namespace SWAN { // The : _numAttributes(0) ect. is an initialization list. It
				 // is a better way to
	// initialize variables, since it avoids an extra copy.
	Shader::Shader()
		: _numAttributes(0), _programID(0), _vertexShaderID(0),
		  _fragmentShaderID(0) {}

	Shader::~Shader() {}

	SWAN_DEBUG_VAR(std::string, dbg_vertexShader);
	SWAN_DEBUG_VAR(std::string, dbg_fragmentShader);

	// Compiles the shaders into a form that your GPU can understand
	void Shader::compileShaders(const std::string& vertexShaderFilePath,
								const std::string& fragmentShaderFilePath) {
		// Create the vertex shader object, and store its ID
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if(_vertexShaderID == 0) {
			std::cerr << "Vertex shader failed to be created!" << std::endl;
		}

		// Create the fragment shader object, and store its ID
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if(_fragmentShaderID == 0) {
			std::cerr << "Fragment shader failed to be created!" << std::endl;
		}

		// Compile each shader
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);

		SWAN_DEBUG_DO(dbg_vertexShader = vertexShaderFilePath);
		SWAN_DEBUG_DO(dbg_fragmentShader = fragmentShaderFilePath);
	}

	void Shader::linkShaders() {
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		_programID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		// Link our program
		glLinkProgram(_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*) &isLinked);
		if(isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(_programID);
			// Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			// print the error log and quit
			std::printf("%s\n", &(errorLog[0]));
			std::cerr << "Shaders failed to link!" << std::endl;
			SWAN_DEBUG_OUT(dbg_vertexShader);
			SWAN_DEBUG_OUT(dbg_fragmentShader);
		}

		// Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	// Adds an attribute to our shader. SHould be called between compiling and
	// linking.
	void Shader::addAttrib(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	// enable the shader, and all its attributes
	void Shader::use() {
		if(!used) {
			glUseProgram(_programID);
			// enable all the attributes we added with addAttribute
			for(int i = 0; i < _numAttributes; i++) {
				glEnableVertexAttribArray(i);
			}
			used = true;
		}
	}

	// disable the shader
	void Shader::unuse() {
		if(used) {
			glUseProgram(0);
			for(int i = 0; i < _numAttributes; i++) {
				glDisableVertexAttribArray(i);
			}
			used = false;
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
		while(std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}

		shaderFile.close();

		// get a pointer to our file contents c string;
		const char* contentsPtr = fileContents.c_str();
		// tell opengl that we want to use fileContents as the contents of the
		// shader file
		glShaderSource(id, 1, &contentsPtr, nullptr);

		// compile the shader
		glCompileShader(id);

		// check for errors
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if(success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			// Print error log and quit
			std::printf("%s\n", &(errorLog[0]));
			std::cerr << "Shader " << filePath << " failed to compile" << std::endl;
		}
	}

	bool Shader::hasUniform(const std::string& name) {
#ifdef SWAN_DEBUG_SHADER
		if(uniforms.find(name) == uniforms.end()) {
			std::cout << "WARNING: Attempting to set uniform variable \"" << name
					  << "\", which doesn't seem to exist, try adding it first\n";
			return false;
		}
		return true;
#else
		return uniforms.find(name) != uniforms.end();
#endif
	}

	void Shader::addUniform(const std::string& name) {
		auto res = glGetUniformLocation(_programID, name.c_str());
		if(res >= 0) {
			uniforms.insert({ name, res });
		} else {
			std::cout << "WARNING: Attempted to add uniform \"" << name
					  << "\", but the uniform doesn't exist\n";
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
