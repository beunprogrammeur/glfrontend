//
// Created by vincent on 19. 4. 25.
//

#include "graphics/resources/wheel_resource.h"

namespace graphics {
namespace resources {


WheelResource::WheelResource(const std::string &name, graphics::drawing::Theme *parent)
        : SystemResource(name, Resource::Type::Wheel, parent)
{

}
} // namespace resources
} // namespace graphics