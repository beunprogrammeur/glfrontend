//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_BGCOLOR_RESOURCE_H
#define GLFRONTEND_BGCOLOR_RESOURCE_H

#include "graphics/resources/system_resource.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace graphics {
namespace resources {

class BGColorResource : public SystemResource
{
private:
    glm::vec4& m_color;
public:
    BGColorResource(const std::string& name, glm::vec4& color);


    GLfloat red()   const { return m_color.r; }
    GLfloat green() const { return m_color.g; }
    GLfloat blue()  const { return m_color.b; }
    glm::vec4& color() { return m_color; }
};

} // namespace resources
} // namespace graphics

#endif //GLFRONTEND_BGCOLOR_RESOURCE_H
