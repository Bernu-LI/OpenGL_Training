#include "ShaderProgram.h"

#include <iostream>

namespace Renderer {
    /* Create a ready-to-use shader program */
    ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragmentShader) {
        /* Vertex shader initialization */
        GLuint vertexShaderID;
        if (!initializeShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
            std::cerr << "Vertex Shader compile time error" << std::endl;
            return;
        }
        
        /* Fragment shader initialization */
        GLuint fragmentShaderID;
        if (!initializeShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
            std::cerr << "Fragment Shader compile time error" << std::endl;
            glDeleteShader(vertexShaderID);     // Delete vertex shader in case of fragment shader compilation error
            return;
        }

        /* Combine independent shader objects into a shader program */
        m_ID = glCreateProgram();       // Create a shader program object and return its unique ID
        glAttachShader(m_ID, vertexShaderID);       // Attach a vertex shader to the program object 
        glAttachShader(m_ID, fragmentShaderID);     // Attach a fragment shader to the program object
        glLinkProgram(m_ID);    // Link the program object

        /* Linking check */
        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);     // Get an integer value about link status
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADE: Link time error:\n" << infoLog << std::endl;    // Get information about shader linking
        }
        else {
            m_isCompiled = true;
        }

        /* Delete no longer needed shader objects */
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
    }


    /* Delete a shader program */
    ShaderProgram::~ShaderProgram() {
        /* Free the memory associated with the shader program object */
        glDeleteProgram(m_ID);
    }

    /* Initialize shader */
    bool ShaderProgram::initializeShader(const char* sourceCode, const GLenum shaderType, GLuint& shaderID) {
        /* Shader initialization */
        shaderID = glCreateShader(shaderType);      // Create a shader object and return its unique ID
        glShaderSource(shaderID, 1, &sourceCode, nullptr);       // Bind source code to the shader object
        glCompileShader(shaderID);      // Compile source code

        /* Compilation check */
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);   // Get an integer value about compilation status
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);   // Get information about shader compilation
            std::cerr << "ERROR::SHADER: Compile time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }
    /* Activate shader program (make it current) */
    void ShaderProgram::use() const {
        glUseProgram(m_ID);
    }

    /* Move a shader program object via assignment operator */
    ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram) {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;

        return *this;
    }
}

