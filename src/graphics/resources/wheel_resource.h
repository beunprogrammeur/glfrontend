//
// Created by vincent on 19. 4. 25.
//

#ifndef GLFRONTEND_WHEEL_RESOURCE_H
#define GLFRONTEND_WHEEL_RESOURCE_H

#include "graphics/resources/system_resource.h"
#include "graphics/drawing/drawable.h"
#include <vector>

namespace graphics {
namespace resources {

template<typename drawable>
class WheelResource : public SystemResource {
private:
    std::vector<drawable*> m_drawables;

public:
    WheelResource(const std::string& name, const std::vector<drawable*>& drawables);
    inline std::vector<drawable*> drawables() { return m_drawables; };
};


template<typename drawable>
WheelResource<drawable>::WheelResource(const std::string &name, const std::vector<drawable*>& drawables)
        : SystemResource(name, Resource::Type::Wheel)
          , m_drawables(drawables)
{
}

} // namespace resources
} // namespace graphice

#endif //GLFRONTEND_WHEEL_RESOURCE_H
