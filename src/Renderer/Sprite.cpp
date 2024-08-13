#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Renderer {
    /* Create a sprite */
    Sprite::Sprite(const std::shared_ptr <Texture2D> pTexture, 
                   const std::shared_ptr <ShaderProgram> pShaderProgram, 
                   const glm::vec2& position, 
                   const glm::vec2& size, 
                   const float rotation)
                   : m_pTexture(std::move(pTexture))
                   , m_pShaderProgram(std::move(pShaderProgram))
                   , m_position(position)
                   , m_size(size)
                   , m_rotation(rotation) {
        /* Array of vertex coordinates in local space */
        const GLfloat vertexCoords[] = {
            // 2--3   1
            // | /  / |
            // 1   3--2

            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,

            1.f, 1.f, 
            1.f, 0.f,
            0.f, 0.f
        };

        /* Array of texture-to-vertex coordinate mapping */
        const GLfloat textureCoords[] {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,

            1.f, 1.f, 
            1.f, 0.f,
            0.f, 0.f
        };
        
        /* Create a Vertex Array Object for vertex attriute state */
        glGenVertexArrays(1, &m_vao);   // Generate and return one unique identifier for a vertex array
        glBindVertexArray(m_vao);       // Create a vertex array object, bind it to the ID & make current

        /* Create a Vertex Buffer Object with vertex coordinate data in video card memory */
        glGenBuffers(1, &m_vertexCoords_vbo);   // Generate and return one unique identifier for a buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoords_vbo);  // Create a buffer of GL_ARRAY_BUFFER type, bind it to the ID & make current
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);  // Fill the buffer with data

        /* Configure the vertex shader to work with coordinates */
        glEnableVertexAttribArray(0);   // Enable use of vertex attribute with index 0 in the vertex array
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the vertex shader interprets data

        /* Create a Vertex Buffer Object with texture-to-vertex coordinate mappiing data in video card memory */
        glGenBuffers(1, &m_textureCoords_vbo);  // Generate and return one unique identifier for a buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_textureCoords_vbo);  // Create a buffer of GL_ARRAY_BUFFER type, bind it to the ID & make current
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);  // Fill the buffer with data 

        /* Configure the vertex shader to work with texture */
        glEnableVertexAttribArray(1);   // Enable use of vertex attribute with index 1 in the vertex array
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // Configure how the vertex shader interprets data

        /* 
        Unbind the current buffer and vertex array. 
        After working with a buffer or vertex array it is a good practice to unbind them
        */
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    /* Delete a sprite */
    Sprite::~Sprite() {
        glDeleteBuffers(1, &m_vertexCoords_vbo);    // Free the memory associated with the buffer object with coordinates
        glDeleteBuffers(1, &m_textureCoords_vbo);   // Free the memory associated with the buffer object with texture-to-vertex coordinate mapping
        glDeleteVertexArrays(1, &m_vao);            // Free the memory associated with the vertex array object
    }

    /* Render a sprite */ 
    void Sprite::render() const {
        /* Activate the shader program (make it current) */
        m_pShaderProgram->use();

        /* 
        Create a model matrix for transformation coordinates from local space to world space. 
        Model matrix determines where the shape is located in OpenGL window
        */
        glm::mat4 modelMatrix(1.f);     // Create a 4x4 identity matrix

        /*  
        Apply some transformations to the model matrix.
        Matrices are multiplied in reverse order, so trasformations are also applied in reverse order
        */
        modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position, 0.f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 1.0f));
        
        /* Link the model matrix to the shader program */
        m_pShaderProgram->setMatrix4("modelMat", modelMatrix);  

        /* Set the texture */
        glActiveTexture(GL_TEXTURE0);   // Activate texture unit 0 (make it current)
        m_pTexture->bind();     // Make the texture current

        /* Make the vertex array current */
        glBindVertexArray(m_vao);

        /* Render objects */
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Unbind the current vertex array and texture */
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /* Set position */
    void Sprite::setPosition(const glm::vec2& position) {
        m_position = position;
    }

    /* Set size */
    void Sprite::setSize(const glm::vec2& size) {
        m_size = size;
    }

    /* Set rotation */
    void Sprite::setRotation(const float rotation) {
        m_rotation = rotation;
    }
}