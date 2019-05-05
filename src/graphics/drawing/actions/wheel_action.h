//
// Created by vincent on 19. 5. 1.
//

#ifndef GLFRONTEND_WHEEL_ACTION_H
#define GLFRONTEND_WHEEL_ACTION_H

#include "graphics/drawing/actions/drawable_action.h"

namespace graphics {
namespace drawing {
namespace actions {

class WheelAction : public DrawableAction
{
private:
    glm::vec2 m_targetDisplacement;
    float m_scrollTotalTime;
    float m_scrollCurrentTime;

public:
    static const std::string kID;

    WheelAction();

    void scrollTime(float time)
    { m_scrollTotalTime = time; }

    float scrollTime() const
    { return m_scrollTotalTime; }

    void update(GLfloat dt) override;
};

} // namespace actions
} // namespace drawing
} // namespace graphics

#endif //GLFRONTEND_WHEEL_ACTION_H
