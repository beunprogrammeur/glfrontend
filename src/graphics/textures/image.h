//
// Created by vincent on 19. 6. 5.
//

#ifndef GLFRONTEND_IMAGE_H
#define GLFRONTEND_IMAGE_H


#include <glad/glad.h>
#include <stb_image.h>

namespace graphics {
namespace textures {

struct Image
{
public:
    GLuint width;
    GLuint height;
    unsigned char *data;
    GLuint internalFormat;
    GLuint imgFormat;

    Image();

    inline void dispose()
    {
        if (data) {
            stbi_image_free(data);
            data = nullptr;
        }
    }
};


} // namespace textures
} // namespace graphics

#endif //GLFRONTEND_IMAGE_H
