//
// Created by vincent on 19. 6. 5.
//
#include "graphics/textures/image.h"

namespace graphics {
namespace textures {

Image::Image()
        : width(0)
          , height(0)
          , data(nullptr)
          , internalFormat(0)
          , imgFormat(0)
{
}

} // namespace textures
} // namespace graphics
