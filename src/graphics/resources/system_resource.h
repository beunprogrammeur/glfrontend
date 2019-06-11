//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_SYSTEM_RESOURCE_H
#define GLFRONTEND_SYSTEM_RESOURCE_H

#include "graphics/resources/resource.h"

namespace graphics {
namespace resources {

class SystemResource : public Resource
{
public:
    SystemResource(const std::string &name, Resource::Type type, graphics::drawing::Theme *parent);
};

} // namespace resources
} // namespace graphics

#endif //GLFRONTEND_SYSTEM_RESOURCE_H
