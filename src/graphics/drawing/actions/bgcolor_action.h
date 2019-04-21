//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_BGCOLOR_ACTION_H
#define GLFRONTEND_BGCOLOR_ACTION_H

#include <glm/vec4.hpp>
#include "graphics/drawing/actions/action.h"

namespace graphics {
namespace drawing {
namespace actions {

class BGColorAction : public Action
{
private:
    glm::vec4 m_destination;
    glm::vec4 m_origin;

public:
    BGColorAction();

    glm::vec4 origin() { return m_origin; }
    void origin(glm::vec4 value) { m_origin = value; }
    void update(glm::vec4& target, GLfloat dt);

    void red(GLfloat value) {m_destination.r = value; }
    void green(GLfloat value) {m_destination.g = value; }
    void blue(GLfloat value) {m_destination.b = value; }

};

} // namespace actions
} // namespace drawing
} // namespace actions

#endif //GLFRONTEND_BGCOLOR_ACTION_H
