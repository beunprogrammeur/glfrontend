//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_FILE_RESOURCE_H
#define GLFRONTEND_FILE_RESOURCE_H

#include "graphics/resources/resource.h"

namespace graphics {
namespace resources {

class FileResource : public Resource
{
private:
    std::string m_path;
    bool m_isLoaded;

protected:
    void markLoaded() { m_isLoaded = true; }

public:
    FileResource(const std::string& path, const std::string& name, Resource::Type type);

    const std::string& path() const    { return m_path; }
    void path(const std::string& path) { m_path = path; }
    bool isLoaded() const  { return m_isLoaded; }

    virtual bool load() = 0;
};

} // namespace resources
} // namespace graphics

#endif //GLFRONTEND_FILE_RESOURCE_H
