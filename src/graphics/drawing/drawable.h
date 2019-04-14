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
    virtual graphics::textures::Texture2D& texture()  = 0;
    virtual const std::string& texturePath() const    = 0;
    virtual void texturePath(const std::string& path) = 0;
};

} // namespace drawing
} // namespace graphics

#endif