#ifndef GRAPHICS_TEXTURE_TEXTURE_H
#define GRAPHICS_TEXTURE_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


namespace graphics
{
namespace textures
{

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

    void generate(const GLuint width, const GLuint height, unsigned char* data, const GLuint internalFormat = GL_RGB, const GLuint imgFormat = GL_RGB);

    inline bool   loaded() const { return m_loaded;}
    inline GLuint width()  const { return m_width;  }
    inline GLuint height() const { return m_height; }
    inline void   bind()   const { glBindTexture(GL_TEXTURE_2D, m_id); };
};

} // namespace textures
} // namespace graphics

#endif
