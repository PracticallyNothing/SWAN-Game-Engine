#pragma once

#include <glm/glm.hpp>

#include <string>
#include <map>

#include <glad/glad.h>

#include "Light.hpp"
#include "../Physics/Transform.hpp"

//This class handles the compilation, linking, and usage of a GLSL shader program.
//Reference: http://www.opengl.org/wiki/Shader_Compilation
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

        GLuint getID(){ return _programID; }

        bool hasUniform(const std::string& name);
        bool hasUniformWarn(const std::string& name);
        void addUniform(const std::string& name);

		GLint getUniformID(const std::string& name);

        void setUniformData(const std::string& name, int    data);
        void setUniformData(const std::string& name, bool   data);
        void setUniformData(const std::string& name, float  data);
        void setUniformData(const std::string& name, double data);

        void setUniformData(const std::string& name, glm::vec2 data);
        void setUniformData(const std::string& name, glm::vec3 data);
        void setUniformData(const std::string& name, glm::vec4 data);

        void setUniformData(const std::string& name, glm::mat2& data, GLboolean transposed = GL_FALSE);
        void setUniformData(const std::string& name, glm::mat3& data, GLboolean transposed = GL_FALSE);
        void setUniformData(const std::string& name, glm::mat4& data, GLboolean transposed = GL_FALSE);

        void setUniformData(const std::string& name, glm::mat2&& data, GLboolean transposed = GL_FALSE);
        void setUniformData(const std::string& name, glm::mat3&& data, GLboolean transposed = GL_FALSE);
        void setUniformData(const std::string& name, glm::mat4&& data, GLboolean transposed = GL_FALSE);

        void setUniformData(const std::string& name, Transform& data);

		void setUniformData(const std::string& name, DirectionalLight light);
		void setUniformData(const std::string& name, PointLight light);
		void setUniformData(const std::string& name, Spotlight light);
    private:
        int _numAttributes;

		void compileShader(const std::string& filePath, GLuint id);

        GLuint _programID;
        GLuint _vertexShaderID;
        GLuint _fragmentShaderID;

        std::map<std::string,GLint> uniforms;
};
