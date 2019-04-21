#include "graphics/resources/resource.h"

namespace graphics
{
namespace resources
{


Resource::Resource(const std::string& name, Type type)
    : m_name(name)
    , m_type(type)
{
}

} // namespace resources
} // namespace graphics