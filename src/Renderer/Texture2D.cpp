#include "Texture2D.h"

namespace Renderer {
    /* Create a ready-to-use texture */
    Texture2D::Texture2D(const GLuint width, const GLuint height, 
        const unsigned char* pixels,
        const unsigned int channels,
        const GLenum filter,
        const GLenum wrapMode)
        : m_width(width), m_height(height) {
            /* Set the format of texture pixel data depending on the number of channels per pixel */
            switch (channels) {
            case 3:
                m_mode = GL_RGB;
                break;
            case 4:
                m_mode = GL_RGBA;
                break;
            default:
                m_mode = GL_RGBA;
                break;
            }

            /* Create a texture */
            glGenTextures(1, &m_ID);    // Generate and return one unique identifier for a texture
            glActiveTexture(GL_TEXTURE0);   // Activate texture unit 0 (make it current)
            glBindTexture(GL_TEXTURE_2D, m_ID);     // Create a texture object, bind it to the ID & make current
            glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, pixels); // Fill the texture with data

            /* Set texture parameters */
            /* Texture wrapping in case texture coordinates are specified out of range */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
            /* Texture filtering in case the texture is smaller or larger the area it is being mapped to on the screen */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

            /* Generate mipmaps for the texture */
            glGenerateMipmap(GL_TEXTURE_2D);

            /*
            Unbind the current texture. 
            After working with a texture it is a good practice to unbind it
            */
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    /* Delete a texture */
    Texture2D::~Texture2D() {
        /* Free the memory associated with the texture */
         glDeleteTextures(1, &m_ID);
    }

    /* Overload move constructor */
    Texture2D::Texture2D(Texture2D&& texture2d) {
        m_ID = texture2d.m_ID;
        texture2d.m_ID = 0;
        m_width = texture2d.m_width;
        m_height = texture2d.m_height;
        m_mode = texture2d.m_mode;
    }

    /* Overload move assignment operator */
    Texture2D& Texture2D::operator = (Texture2D&& texture2d) {
        glDeleteTextures(1, &m_ID);
        m_ID = texture2d.m_ID;
        texture2d.m_ID = 0;
        m_width = texture2d.m_width;
        m_height = texture2d.m_height;
        m_mode = texture2d.m_mode;
        return *this;
    }

    /* Make the texture current */
    void Texture2D::bind() const {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    /* Add a subtexture(tile) */
    void Texture2D::addSubTexture(std::string subTextureName, const glm::vec2& leftBottomUV, glm::vec2& rigthTopUV) {
        /* Create a subtexture object and emplace it in the map */
        m_subTextures.emplace(std::move(subTextureName), SubTexture2D(leftBottomUV, rigthTopUV));
    }

    /* Get subtexture by its name */
    const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& subTextureName) const {
        auto it = m_subTextures.find(subTextureName);
        /* Check the existence of the subtexture. If the subtexture does not exist, then take the entire texture as subtexture */
        if (it == m_subTextures.end()) {
            const static SubTexture2D defaultSubTexture;
            return defaultSubTexture;
        }
        return it->second;
    }
    
}