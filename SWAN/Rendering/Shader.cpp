#include "Shader.hpp"

#include "Core/Format.hpp"
#include "Core/Logging.hpp"

#include "Utility/Debug.hpp"
#include "Utility/Group.hpp"

#include <fstream>
#include <iostream>
#include <vector>

namespace SWAN
{
	SWAN_DEBUG_VAR(std::string, dbg_vertexShader);
	SWAN_DEBUG_VAR(std::string, dbg_fragmentShader);

	static Shader* ActiveShader = nullptr;

	void Shader::compileShaders(const std::string& vertexShaderFilepath,
	                            const std::string& fragmentShaderFilepath)
	{
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		if(!vertexShaderID)
			Log("Shader",
			    Format("Vertex shader \"{}\" failed to be created!",
			           vertexShaderFilepath),
			    LogLevel::Error);
		if(!fragmentShaderID)
			Log("Shader",
			    Format("Fragment shader \"{}\" failed to be created!",
			           fragmentShaderFilepath),
			    LogLevel::Error);

		compileShader(vertexShaderFilepath, vertexShaderID);
		compileShader(fragmentShaderFilepath, fragmentShaderID);

		SWAN_DEBUG_DO(dbg_vertexShader = vertexShaderFilepath);
		SWAN_DEBUG_DO(dbg_fragmentShader = fragmentShaderFilepath);
	}

	void Shader::linkShaders()
	{
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

	void Shader::addAttrib(const std::string& attributeName)
	{
		glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
	}

	void Shader::use()
	{
		if(this != ActiveShader) {
			glUseProgram(programID);
			for(int i = 0; i < numAttributes; i++)
				glEnableVertexAttribArray(i);
			ActiveShader = this;
		}
	}

	// disable the shader
	void Shader::unuse()
	{
		if(this == ActiveShader) {
			glUseProgram(0);
			for(int i = 0; i < numAttributes; i++)
				glDisableVertexAttribArray(i);
			ActiveShader = nullptr;
		}
	}

	void Shader::compileShadersFromSrc(const char* vertSrc, const char* fragSrc)
	{
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		comp(vertSrc, vertexShaderID);
		comp(fragSrc, fragmentShaderID);
	}

	void Shader::comp(const char* src, GLuint id)
	{
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
	void Shader::compileShader(const std::string& filePath, GLuint id)
	{
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

	bool Shader::hasUniform(const std::string& name)
	{
#ifdef SWAN_DEBUG_SHADER
		if(uniforms.find(name) == uniforms.end()) {
			Log("Shader",
			    Format("Attempting to set uniform variable \"{}\", which doesn't seem to exist, try adding it first.", name),
			    LogLevel::Warning);
			return false;
		}
		return true;
#else
		return uniforms.find(name) != uniforms.end();
#endif
	}

	void Shader::addUniform(const std::string& name)
	{
		auto res = glGetUniformLocation(programID, name.c_str());
		if(res >= 0) {
			uniforms.insert({ name, res });
		} else {
			Log("Shader",
			    Format("Attempted to add uniform \"{}\", but the uniform doesn't exist.", name),
			    LogLevel::Error);
		}
	}

	GLint Shader::getUniformID(const std::string& name) { return uniforms[name]; }

	void Shader::SetInt(const String& name, int data)
	{
		if(hasUniform(name))
			glUniform1i(uniforms[name], data);
	}

	void Shader::SetReal(const String& name, double data)
	{
		if(hasUniform(name))
			glUniform1f(uniforms[name], data);
	}

	void Shader::SetVec2(const std::string& name, vec2 data)
	{
		if(hasUniform(name))
			glUniform2f(uniforms[name], data.x, data.y);
	}

	void Shader::SetVec3(const std::string& name, vec3 data)
	{
		if(hasUniform(name))
			glUniform3f(uniforms[name], data.x, data.y, data.z);
	}

	void Shader::SetVec4(const std::string& name, vec4 data)
	{
		if(hasUniform(name))
			glUniform4f(uniforms[name], data.x, data.y, data.z, data.w);
	}

	void Shader::SetMat2(const std::string& name, mat2 data, GLboolean transposed)
	{
		if(hasUniform(name))
			glUniformMatrix2fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
	}

	void Shader::SetMat3(const std::string& name, mat3 data, GLboolean transposed)
	{
		if(hasUniform(name))
			glUniformMatrix3fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
	}

	void Shader::SetMat4(const std::string& name, mat4 data, GLboolean transposed)
	{
		if(hasUniform(name))
			glUniformMatrix4fv(uniforms[name], 1, transposed, (const GLfloat*) data.data.data());
	}
} // namespace SWAN
