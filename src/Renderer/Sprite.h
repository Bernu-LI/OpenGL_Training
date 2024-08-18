#pragma once

#include <glm/vec2.hpp>
#include <glad/glad.h>

#include <memory>
#include <string>

namespace Renderer {
    class Texture2D;
    class ShaderProgram;

    class Sprite {
    public:
        /* Create a sprite */
        Sprite(const std::shared_ptr <Texture2D> pTexture, 
               const std::string initialSubTextureName,
               const std::shared_ptr <ShaderProgram> pShaderProgram, 
               const glm::vec2& position = glm::vec2(0.f), 
               const glm::vec2& size = glm::vec2(1.f), 
               const float rotation = 0.f);

        /* Delete a sprite */
        ~Sprite();

        /* Prohibit copying of sprite objects */
        Sprite(const Sprite&) = delete;
        Sprite& operator = (const Sprite&) = delete;

        /* Render a sprite */
        void render() const;

        /* Set position */
        void setPosition(const glm::vec2& position);

        /* Set size */
        void setSize(const glm::vec2& size);

        /* Set rotation */
        void setRotation(const float rotation);

    private:
        std::shared_ptr <Texture2D> m_pTexture;
        std::shared_ptr <ShaderProgram> m_pShaderProgram;
        glm::vec2 m_position;
        glm::vec2 m_size;
        float m_rotation;

        GLuint m_vao;
        GLuint m_vertexCoords_vbo;
        GLuint m_textureCoords_vbo;
    };
}