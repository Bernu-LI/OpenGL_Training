#pragma once

#include <glad/glad.h>

#include <string>

namespace Renderer {
    class ShaderProgram {
    public:
        /* Create a ready-to-use shader program */
        ShaderProgram(const char* vertexShader, const char* fragmentShader);

        /* Delete a shader program */
        ~ShaderProgram();

        bool isCompiled() const {
            return m_isCompiled;
        }

        /* Activate shader program (make it current) */
        void use() const;
        
        /* Prohibit assignment-operator-based copying of shader program objects */
        ShaderProgram& operator = (const ShaderProgram&) = delete;
        /* Allow assignment-operator-based moving of shader program objects */
        ShaderProgram& operator = (ShaderProgram&& ShaderProgram);

    private:
        bool m_isCompiled = false;
        GLuint m_ID = 0;
        
        /* Initialize shader */
        bool initializeShader(const char* sourceCode, const GLenum shaderType, GLuint& shaderID);
    }; 
}