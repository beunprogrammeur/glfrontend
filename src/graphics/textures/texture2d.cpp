#include <stb_image.h>
#include "graphics/textures/texture2d.h"

namespace graphics {
namespace textures {
Texture2D::Texture2D()
        : m_id(0)
          , m_width(0)
          , m_height(0)
          , m_loaded(false)
{
}

Texture2D::~Texture2D()
{
    // TODO: free up the texture id if we want to
    //if (m_id != 0) {
    //    glDeleteTextures(1, &m_id);
    //}
}


void Texture2D::generate(const GLuint width, const GLuint height, unsigned char *data, const GLuint internalFormat,
                         const GLuint imgFormat)
{
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_id);

    // building the texture
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data);

    // set texture parameters (how to handle borders and interpolation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(m_id);

    // unbind for safety 
    glBindTexture(GL_TEXTURE_2D, 0);
    m_loaded = true;
}


void Texture2D::generate(Image &image)
{
    generate(image.width, image.height, image.data, image.internalFormat, image.imgFormat);
    if (image.data != nullptr) {
        stbi_image_free(image.data);
        image.data = nullptr;
    }
}


} // namespace textures
} // namespace graphics