//
// Created by vincent on 19. 6. 5.
//

#ifndef GLFRONTEND_IMAGE_H
#define GLFRONTEND_IMAGE_H


#include <glad/glad.h>

namespace graphics {
namespace textures {

struct Image
{
public:
    GLuint width;
    GLuint height;
    unsigned char* data;
    GLuint internalFormat;
    GLuint imgFormat;

    Image();
};


} // namespace textures
} // namespace graphics

#endif //GLFRONTEND_IMAGE_H
