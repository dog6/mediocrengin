#include <glad/glad.h>
#include "AVGNG/Graphics/Shader.hpp"

using namespace ng::Core;
using namespace ng::Assets;

namespace ng::Graphics {


	// Constrctor & Destructor
    Shader::Shader() {}
    Shader::~Shader() {  }

    // Shader Methods
    void Shader::Build(const char* vertexCode, const char* fragmentCode)
    {
        int success;
        char infoLog[512];
        
        Debug::Log(DEBUG, "Building shader program...");

        // VERTEX SHADER
        //
        // Compile vertex shader
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, NULL);
        glCompileShader(vertex);

        // Check vertex shader
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            Debug::Log(LogLevel::ERROR, "ERROR: Vertex shader compilation failed\n%s\n", infoLog);
            return;
        }
        
        // FRAGMENT SHADER
        //
        // Compile fragment shader
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment, 1, &fragmentCode, NULL);
        glCompileShader(fragment);

        // Check fragment shader
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            Debug::Log(LogLevel::ERROR, "ERROR: Fragment shader compilation failed\n%s", infoLog);
            return;
        }

        // Link program
        ID = glCreateProgram();
        Debug::Log(DEBUG, "Created shader program with ID: %d", ID);

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // Check linking
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            Debug::Log(LogLevel::ERROR, "ERROR: Shader program linking failed\n%s\n", infoLog);
            return;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        Debug::Log(LogLevel::DEBUG, "Shader created successfully! ID: %d\n", ID);
    }

    void Shader::Use()
    {
        // HUGE PERFORMANCE LOSS!!
        // Currently calling every frame, even if this shader is already bound
        
        // Flush all existing errors first so we don't catch old bugs
        while (glGetError() != GL_NO_ERROR);

        // Check if shader ID is valid
        if (this->ID == 0) {
            Debug::Log(ERROR, "Trying to use shader with ID 0!");
            return;
        }

        // Check lua script validity
        if (!glIsProgram(ID)) {
            Debug::Log(ERROR, "Shader ID %d is not a valid shader program!", ID);
            return;
        }

        glValidateProgram(ID);
        GLint status;
        glGetProgramiv(ID, GL_VALIDATE_STATUS, &status);
        if (status == GL_FALSE) {
            char infoLog[512];
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            Debug::Log(ERROR, "Shader Validation Failed: %s", infoLog);
        }

         //Debug::Log(DEBUG, "Using shader program ID : % d", ID);
        glUseProgram(ID);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            Debug::Log(ERROR, "Error using shader %d: %d", ID, err);
        }
	
    }

    void Shader::SetMat4(const char* name, glm::mat4 matrix)
    {
        GLint location = glGetUniformLocation(ID, name);
        if (location == -1) {
            Debug::Log(WARN, "Uniform '%s' not found in shader %d", name, ID);
            return;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetVec3(const char* name, glm::vec3 vec)
    {
        GLint location = glGetUniformLocation(ID, name);
        if (location == -1) {
            Debug::Log(WARN, "Uniform '%s' not found in shader %d", name, ID);
            return;
        }
        glUniform3f(location, vec.x, vec.y, vec.z);
    }

    void Shader::SetVec4(const char* name, glm::vec4 vec)
    {
        glUniform4f(glGetUniformLocation(ID, name), vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::SetInt(const char* name, int v)
    {
        GLint location = glGetUniformLocation(ID, name);
        if (location == -1) {
            Debug::Log(WARN, "Uniform '%s' not found in shader %d", name, ID);
            return;
        }
        glUniform1i(location, v);
    }

    void Shader::SetFloat(const char* name, float v)
    {
		GLint location = glGetUniformLocation(ID, name);
		if (location == -1) {
			Debug::Log(WARN, "Uniform '%s' not found in shader %d", name, ID);
			return;
		}
		glUniform1f(location, v);
    }

    void Shader::SetBool(const char* name, bool v) {
        GLint location = glGetUniformLocation(ID, name);
        if (location == -1) {
            Debug::Log(WARN, "Uniform '%s' not found in shader %d", name, ID);
            return;
        }
        glUniform1i(location, (int)v);
    }
    
   

}