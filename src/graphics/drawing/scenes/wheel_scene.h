//
// Created by vincent on 19. 5. 1.
//

#ifndef GLFRONTEND_WHEEL_SCENE_H
#define GLFRONTEND_WHEEL_SCENE_H

#include "graphics/drawing/scenes/scene.h"
#include "graphics/drawing/dimensions.h"

namespace graphics {

namespace textures {
class Renderer;
}

namespace drawing {
namespace scenes {

class WheelScene : public Scene
{
private:
    Dimensions m_selectedPosition;
    Dimensions m_drawingPosition;
    glm::vec2 m_transitionDispositionTarget;
    glm::vec2 m_transitionDispositionCurrent;
    int m_internalDrawingIndex;
    bool m_indexInitialized;

public:
    WheelScene(const std::string &name, const std::string &resource);

    void update(GLfloat dt) override;

    void draw(graphics::textures::Renderer &renderer) override;

    Dimensions &dimensions()
    { return m_selectedPosition; }

    inline glm::vec2 transitionDispositionTarget()
    { return m_transitionDispositionTarget; }

    inline void transitionDispositionCurrent(glm::vec2 value)
    { m_transitionDispositionCurrent = value; }

    inline glm::vec2 transitionDispositionCurrent() const
    { return m_transitionDispositionCurrent; }

    inline void internalDrawingIndex(int index)
    { m_internalDrawingIndex = index; }

    void addAction(actions::Action *action) override;

};

} // namespace scenes
} // namespace drawing
} // namespace graphics



#endif //GLFRONTEND_WHEEL_SCENE_H
