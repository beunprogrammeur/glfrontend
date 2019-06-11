#ifndef GRAPHICS_RESOURCES_RESOURCE_H
#define GRAPHICS_RESOURCES_RESOURCE_H

#include <string>
#include "graphics/drawing/theme.h"

namespace graphics {
namespace resources {

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
    graphics::drawing::Theme *m_parent;

protected:
    graphics::drawing::Theme *parent()
    { return m_parent; }

public:
    Resource(const std::string &name, Type type, graphics::drawing::Theme *parent);

    virtual ~Resource() = default;

    const std::string &name() const
    { return m_name; }

    Resource::Type type() const
    { return m_type; }
};

} // namespace resources
} // namespace graphics


#endif