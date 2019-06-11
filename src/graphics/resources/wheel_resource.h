//
// Created by vincent on 19. 4. 25.
//

#ifndef GLFRONTEND_WHEEL_RESOURCE_H
#define GLFRONTEND_WHEEL_RESOURCE_H

#include <vector>

#include "graphics/resources/system_resource.h"
#include "graphics/drawing/drawable.h"
#include "graphics/drawing/theme.h"

#include "filesystem/database/entity/texture_wrapper.h"

namespace graphics {

namespace resources {

class WheelResource : public SystemResource
{

public:
    WheelResource(const std::string &name, graphics::drawing::Theme *parent);

    //inline std::vector<filesystem::database::entity::TextureWrapper *> drawables()
    //{ return parent()->drawables(); }
};

} // namespace resources
} // namespace graphice

#endif //GLFRONTEND_WHEEL_RESOURCE_H
