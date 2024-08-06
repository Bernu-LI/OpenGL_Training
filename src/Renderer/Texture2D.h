#pragma once
 
#include <glad/glad.h>
#include <string>

namespace Renderer {
    class Texture2D {
    public:
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

    private:
        GLuint m_ID;
        unsigned int m_width;
        unsigned int m_height;
        GLenum m_mode;
    };
}
 