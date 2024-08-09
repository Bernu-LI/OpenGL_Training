#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <string>

namespace Renderer {
    class ShaderProgram {
    public:
        /* Create a ready-to-use shader program */
        ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

        /* Delete a shader program */
        ~ShaderProgram();

        bool isCompiled() const {
            return m_isCompiled;
        }

        /* Activate shader program (make it current) */
        void use() const;
        
        /* Link a texture to a shader program */
        void setTexture(const std::string& textureName, const GLint textureUnit);

        /* Link a matrix to a shader program */
        void setMatrix4(const std::string& matrixName, const glm::mat4& matrix);
        
        /*
        Overload assignment-operator-based moving of shader program objects.
        Declaring a custom move assingment operator implicitly deletes default copy constructor and copy assignment operator
        */ 
        ShaderProgram& operator = (ShaderProgram&& ShaderProgram);

    private:
        bool m_isCompiled = false;
        GLuint m_ID = 0;
        
        /* Initialize shader */
        bool initializeShader(const std::string& sourceCode, const GLenum shaderType, GLuint& shaderID);
    }; 
}