//
// Created by vincent on 19. 4. 21.
//

#include "graphics/resources/file_resource.h"


namespace graphics {
namespace resources {


FileResource::FileResource(const std::string &path, const std::string &name, Resource::Type type)
    : Resource(name, type)
      , m_path(path)
      , m_isLoaded(false)
{

}

} // namespace resources
} // namespace graphics