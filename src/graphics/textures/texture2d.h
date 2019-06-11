#ifndef GRAPHICS_TEXTURE_TEXTURE_H
#define GRAPHICS_TEXTURE_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "graphics/textures/image.h"


namespace graphics {
namespace textures {

class Texture2D
{
private:
    GLuint m_id;
    GLuint m_width;
    GLuint m_height;
    bool m_loaded;

public:
    Texture2D();

    ~Texture2D();

    void generate(const GLuint width, const GLuint height, unsigned char *data, const GLuint internalFormat = GL_RGB,
                  const GLuint imgFormat = GL_RGB);

    void generate(Image &image);

    inline bool loaded() const
    { return m_loaded; }

    inline GLuint width() const
    { return m_width; }

    inline GLuint height() const
    { return m_height; }

    inline void bind() const
    { glBindTexture(GL_TEXTURE_2D, m_id); };

    inline void dispose()
    {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }
};

} // namespace textures
} // namespace graphics

#endif
