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
    explicit SystemResource(const std::string& name, Type type);
};

} // namespace resources
} // namespace graphics

#endif //GLFRONTEND_SYSTEM_RESOURCE_H
