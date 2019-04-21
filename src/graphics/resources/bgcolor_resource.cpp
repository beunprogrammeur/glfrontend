//
// Created by vincent on 19. 4. 21.
//

#include "bgcolor_resource.h"

namespace graphics {
namespace resources {

BGColorResource::BGColorResource(const std::string &name, glm::vec4 &color)
        : SystemResource(name, graphics::resources::Resource::Type::BGColor)
          , m_color(color)
{
}

} // namespace resources
} // namespace graphics