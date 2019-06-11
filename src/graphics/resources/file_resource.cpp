//
// Created by vincent on 19. 4. 21.
//

#include "graphics/resources/file_resource.h"


namespace graphics {
namespace resources {

FileResource::FileResource(const std::string &path, const std::string &name, Resource::Type type,
                           graphics::drawing::Theme *parent)
        : Resource(name, type, parent)
          , m_path(path)
          , m_isLoaded(false)

{

}
} // namespace resources
} // namespace graphics