#ifndef GRAPHICS_DRAWABLE_H
#define GRAPHICS_DRAWABLE_H

#include "graphics/textures/texture2d.h"

namespace graphics
{
namespace drawing
{
    
class Drawable
{
public:
    inline virtual graphics::textures::Texture2D& texture() = 0;
};

} // namespace drawing
} // namespace graphics

#endif