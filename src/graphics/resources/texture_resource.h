#ifndef GRAPHICS_RESOURCES_TEXTURE_RESOURCE_H
#define GRAPHICS_RESOURCES_TEXTURE_RESOURCE_H

#include "graphics/resources/resource.h"
#include "graphics/textures/texture2d.h"

namespace graphics
{
namespace resources
{

class TextureResource : public Resource
{
private:
    graphics::textures::Texture2D m_texture;
public:
    TextureResource(std::string& path, std::string& name) : Resource(path, name) { }
    bool load();
    graphics::textures::Texture2D& texture() { return m_texture; }
};

} // namespace resources
} // namespace graphics


#endif