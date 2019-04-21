//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_DRAWABLE_RESOURCE_H
#define GLFRONTEND_DRAWABLE_RESOURCE_H

#include "graphics/textures/texture2d.h"
#include "graphics/resources/file_resource.h"

namespace graphics
{
namespace resources {

class DrawableResource : public FileResource
{
public:
    DrawableResource(const std::string& path, const std::string& name, graphics::resources::Resource::Type type)
        : FileResource(path, name, type)
    {}

    virtual bool load() = 0;
    virtual graphics::textures::Texture2D& texture() = 0;
};

} // namespace resources
} // namespace graphics

#endif //GLFRONTEND_DRAWABLE_RESOURCE_H
