#include "graphics/resources/resource.h"

namespace graphics {
namespace resources {

Resource::Resource(const std::string &name, Resource::Type type, graphics::drawing::Theme *parent)
        : m_name(name)
          , m_type(type)
          , m_parent(parent)
{
}
} // namespace resources
} // namespace graphics