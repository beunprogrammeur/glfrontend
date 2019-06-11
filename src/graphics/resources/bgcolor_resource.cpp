//
// Created by vincent on 19. 4. 21.
//

#include "bgcolor_resource.h"

namespace graphics {
namespace resources {

BGColorResource::BGColorResource(const std::string &name, glm::vec4 &color, graphics::drawing::Theme* parent)
        : SystemResource(name, graphics::resources::Resource::Type::BGColor, parent)
          , m_color(color)
{
}

} // namespace resources
} // namespace graphics