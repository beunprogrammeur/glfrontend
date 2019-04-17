#ifndef GRAPHICS_RESOURCES_RESOURCE_H
#define GRAPHICS_RESOURCES_RESOURCE_H

#include <string>

namespace graphics
{
namespace resources
{

class Resource
{
private:
    std::string m_path;
    std::string m_name;
    bool m_isLoaded;
protected:
    void markLoaded() { m_isLoaded = true; }

public:
    Resource(std::string& path, std::string& name);

    const std::string& path() const    { return m_path; }
    void path(const std::string& path) { m_path = path; }
    const std::string& name() const { return m_name; }
    bool isLoaded() const  { return m_isLoaded; }
    
    virtual bool load() = 0;
};

} // namespace resources
} // namespace graphics


#endif