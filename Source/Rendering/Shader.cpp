#include "Shader.hpp"

#include "../Utility/Debug.hpp"

#include <fstream>
#include <iostream>
#include <vector>

// The : _numAttributes(0) ect. is an initialization list. It is a better way to
// initialize variables, since it avoids an extra copy.
Shader::Shader()
	: _numAttributes(0),
	_programID(0),
	_vertexShaderID(0),
	_fragmentShaderID(0)
{}

Shader::~Shader() {}

DEBUG_VAR(std::string, dbg_vertexShader);
DEBUG_VAR(std::string, dbg_fragmentShader);

// Compiles the shaders into a form that your GPU can understand
void Shader::compileShaders(const std::string& vertexShaderFilePath,
		const std::string& fragmentShaderFilePath) {
	// Create the vertex shader object, and store its ID
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		std::cerr << "Vertex shader failed to be created!" << std::endl;
	}

	// Create the fragment shader object, and store its ID
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		std::cerr << "Fragment shader failed to be created!" << std::endl;
	}

	// Compile each shader
	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);

	DEBUG_DO(dbg_vertexShader = vertexShaderFilePath);
	DEBUG_DO(dbg_fragmentShader = fragmentShaderFilePath);
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
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
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
		DEBUG_OUT(dbg_vertexShader);
		DEBUG_OUT(dbg_fragmentShader);
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
	glUseProgram(_programID);
	// enable all the attributes we added with addAttribute
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

// disable the shader
void Shader::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

// Compiles a single shader file
void Shader::compileShader(const std::string& filePath, GLuint id) {
	// Open the file
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		std::cerr << "Failed to open " + filePath << std::endl;
	}

	// File contents stores all the text in the file
	std::string fileContents = "";
	// line is used to grab each line of the file
	std::string line;

	// Get all the lines in the file and add it to the contents
	while (std::getline(shaderFile, line)) {
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

	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id);  // Don't leak the shader.

		// Print error log and quit
		std::printf("%s\n", &(errorLog[0]));
		std::cerr << "Shader " << filePath << " failed to compile" << std::endl;
	}
}

bool Shader::hasUniform(const std::string& name) {
#ifdef DEBUG_SHADER
	if(uniforms.find(name) == uniforms.end()){
		std::cout << "WARNING: Attempting to set uniform variable \"" << name << "\", which doesn't seem to exist, try adding it first\n";
		return false;
	}
	return true;
#else
	return uniforms.find(name) != uniforms.end();
#endif
}

void Shader::addUniform(const std::string& name) {
	auto res = glGetUniformLocation(_programID, name.c_str());
	if(res >= 0){
		uniforms.insert({name, res});
	} else {
		std::cout << "WARNING: Attempted to add uniform \"" << name << "\", but the uniform doesn't exist\n";
	}
}

GLint Shader::getUniformID(const std::string& name) { return uniforms[name]; }

void Shader::setUniformData(const std::string& name, int data) {
	if(hasUniform(name)) glUniform1i(uniforms[name], data);
}

void Shader::setUniformData(const std::string& name, float data) {
	if(hasUniform(name)) glUniform1f(uniforms[name], data);
}

void Shader::setUniformData(const std::string& name, double data) {
	if(hasUniform(name)) glUniform1f(uniforms[name], data);
}

void Shader::setUniformData(const std::string& name, glm::vec2 data) {
	if(hasUniform(name)) glUniform2f(uniforms[name], data.x, data.y);
}

void Shader::setUniformData(const std::string& name, glm::vec3 data) {
	if(hasUniform(name))
		glUniform3f(uniforms[name], data.x, data.y, data.z);
}

void Shader::setUniformData(const std::string& name, glm::vec4 data) {
	if(hasUniform(name))
		glUniform4f(uniforms[name], data.x, data.y, data.z, data.w);
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat2& data,
		                    GLboolean transposed) {
	if(hasUniform(name))
		glUniformMatrix2fv(uniforms[name], 1, transposed, &data[0][0]);
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat3& data,
		                    GLboolean transposed) {
	if(hasUniform(name))
		glUniformMatrix3fv(uniforms[name], 1, transposed, &data[0][0]);
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat4& data,
	                    	GLboolean transposed) {
	if(hasUniform(name))
		glUniformMatrix4fv(uniforms[name], 1, transposed, &data[0][0]);
}

void Shader::setUniformData(const std::string& name, Transform& data) {
	if(hasUniform(name)) {
		glm::mat4 buff = data.getModel();
		setUniformData(name, buff);
	}
}

void Shader::setUniformData(const std::string& name, bool data) {
	if(hasUniform(name))
		setUniformData(name, data ? 1 : 0);
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat2&& data,
	                    	GLboolean transposed) {
	if(hasUniform(name)) {
		glm::mat2 _m = data;
		setUniformData(name, _m, transposed);
	}
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat3&& data,
		                    GLboolean transposed) {
	if(hasUniform(name)) {
		glm::mat3 _m = data;
		setUniformData(name, _m, transposed);
	}
}

void Shader::setUniformData(const std::string& name,
		                    glm::mat4&& data,
		                    GLboolean transposed) {
	if(hasUniform(name)){
		glm::mat4 _m = data;
		setUniformData(name, _m, transposed);
	}
}


void Shader::setUniformData(const std::string& name, DirectionalLight light) {
	setUniformData(name + ".dir", light.direction);

	setUniformData(name + ".type", L_DIRECTIONAL);

	setUniformData(name + ".ambient",  light.ambient);
	setUniformData(name + ".diffuse",  light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt",   light.linearAtt);
	setUniformData(name + ".quadAtt",  light.quadraticAtt);
}

void Shader::setUniformData(const std::string& name, PointLight light){
	setUniformData(name + ".pos", light.position);

	setUniformData(name + ".type", L_POINT);

	setUniformData(name + ".ambient",  light.ambient);
	setUniformData(name + ".diffuse",  light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt",   light.linearAtt);
	setUniformData(name + ".quadAtt",  light.quadraticAtt);
}

void Shader::setUniformData(const std::string& name, Spotlight light){
	setUniformData(name + ".pos", light.position);
	setUniformData(name + ".dir", light.direction);

	setUniformData(name + ".cutoff", 	  (float) light.cutoff);
	setUniformData(name + ".outerCutoff", (float) light.outerCutoff);

	setUniformData(name + ".type", L_SPOT);

	setUniformData(name + ".ambient",  light.ambient);
	setUniformData(name + ".diffuse",  light.diffuse);
	setUniformData(name + ".specular", light.specular);

	setUniformData(name + ".constAtt", light.constAtt);
	setUniformData(name + ".linAtt",   light.linearAtt);
	setUniformData(name + ".quadAtt",  light.quadraticAtt);
}
