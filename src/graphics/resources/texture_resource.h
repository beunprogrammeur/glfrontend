#ifndef GRAPHICS_RESOURCES_TEXTURE_RESOURCE_H
#define GRAPHICS_RESOURCES_TEXTURE_RESOURCE_H

#include "graphics/resources/drawable_resource.h"
#include "graphics/textures/texture2d.h"

namespace graphics
{
namespace resources
{

class TextureResource : public DrawableResource
{
private:
    graphics::textures::Texture2D m_texture;
public:
    TextureResource(std::string& path, std::string& name) : DrawableResource(path, name, graphics::resources::Resource::Type::Texture) { }
    bool load() override;
    graphics::textures::Texture2D& texture() { return m_texture; }
};

} // namespace resources
} // namespace graphics


#endif