#include "graphics/resources/resource.h"

namespace graphics
{
namespace resources
{


Resource::Resource(std::string& path, std::string& name)
    : m_path(path)
    , m_name(name)
    , m_isLoaded(false)
{
}

} // namespace resources
} // namespace graphics