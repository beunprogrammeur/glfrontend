#ifndef GRAPHICS_RESOURCES_RESOURCE_H
#define GRAPHICS_RESOURCES_RESOURCE_H

#include <string>

namespace graphics
{
namespace resources
{

class Resource
{
public:
    enum class Type
    {
        Unknown,
        Texture,
        Video,
        SFX,
        Wheel,
        BGColor
    };

private:
    std::string m_name;
    Type m_type;

public:
    explicit Resource(const std::string& name, Type type);
    virtual ~Resource() = default;

    const std::string& name() const { return m_name; }
    Resource::Type type() const { return m_type; }
};

} // namespace resources
} // namespace graphics


#endif