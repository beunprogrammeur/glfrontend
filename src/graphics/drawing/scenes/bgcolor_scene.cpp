//
// Created by vincent on 19. 4. 21.
//


#include "graphics/drawing/scenes/bgcolor_scene.h"
#include "graphics/drawing/actions/bgcolor_action.h"
namespace graphics {
namespace drawing {
namespace scenes {

BGColorScene::BGColorScene(const std::string& name, const std::string& resource)
    : Scene(name, resource)
{
}

void BGColorScene::draw(graphics::textures::Renderer &renderer, graphics::resources::BGColorResource &resource)
{
    resource.color() = m_color;
}

void BGColorScene::update(GLfloat dt)
{
    if (m_currentActionId.empty()) {
        return;
    }
    actions::BGColorAction* action = static_cast<actions::BGColorAction*>(m_actions[m_currentActionId]);


        dt *= 1000; // the domain here is milliseconds in stead of seconds
        action->update(m_color, dt);


        if (action->targetReached()) {
            action->resetTime();
            m_currentActionId = action->next();
            static_cast<actions::BGColorAction*>(m_actions[m_currentActionId])->origin(m_color);
        }
}

} // namespace scenes
} // namespace drawing
} // namespace graphics