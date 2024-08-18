#pragma once
 
#include <glad/glad.h>
#include <string>
#include <glm/vec2.hpp>
#include <map>

namespace Renderer {
    class Texture2D {
    public:
        /* Structure for storing coordiniates of subtextures(tiles) */
        struct SubTexture2D {
            SubTexture2D(const glm::vec2& leftBottomUV_, const glm::vec2& rightTopUV_)
                : leftBottomUV(leftBottomUV_), rightTopUV(rightTopUV_) 
            {}

            SubTexture2D()
                : leftBottomUV(0.f), rightTopUV(1.f)
            {}

            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;
        };

        /* Create a ready-to-use texture */
        Texture2D(const GLuint width, const GLuint height, 
                  const unsigned char* pixels,
                  const unsigned int channels = 4,
                  const GLenum filter = GL_LINEAR,
                  const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        /* Delete a texture */
        ~Texture2D();
        
        /*
        Overload move constructor and move assignment operator.
        Overloading at least one of them implicitly deletes default copy constructor and copy assignment operator
        */
        Texture2D(Texture2D&& texture2d);
        Texture2D& operator = (Texture2D&& texture2d);

        /* Make the texture current */
        void bind() const;

        /* Add a subtexture(tile) */
        void addSubTexture(std::string subTextureName, const glm::vec2& leftBottomUV, glm::vec2& rigthTopUV);
        /* Get subtexture by its name */
        const SubTexture2D& getSubTexture(const std::string& subTextureName) const;

        /* Getters for the width and height of the texture */
        unsigned int width() const { return m_width; }
        unsigned int height() const { return m_height; }

    private:
        GLuint m_ID;
        unsigned int m_width;
        unsigned int m_height;
        GLenum m_mode;

        std::map <std::string, SubTexture2D> m_subTextures;
    };
}
 