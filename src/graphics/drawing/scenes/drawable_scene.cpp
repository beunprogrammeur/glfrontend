//
// Created by vincent on 19. 4. 21.
//

#include <graphics/drawing/actions/drawable_action.h>
#include "drawable_scene.h"

namespace graphics {
namespace drawing {
namespace scenes {

DrawableScene::DrawableScene(const std::string &name, const std::string &resource)
        : Scene(name, resource)
          , m_dimensions()
{

}

void DrawableScene::draw(graphics::textures::Renderer &renderer, graphics::resources::DrawableResource &resource)
{
    renderer.draw(resource.texture(), m_dimensions.position, m_dimensions.size, m_dimensions.angle,
                  glm::vec3(1.0f, 1.0f, 1.0f), m_dimensions.opacity);
}

void DrawableScene::update(GLfloat dt)
{
    if (m_currentActionId.empty()) {
        return;
    }
    auto *action = static_cast<actions::DrawableAction *>(m_actions[m_currentActionId]);


    dt *= 1000; // the domain here is milliseconds in stead of seconds
    action->update(m_dimensions, dt);


    if (action->targetReached()) {
        action->resetTime();
        m_currentActionId = action->next();
        static_cast<actions::DrawableAction *>(m_actions[m_currentActionId])->origin(m_dimensions);
    }
}

} // namespace scenes
} // namespace drawing
} // namespace graphics
