//
// Created by vincent on 19. 4. 21.
//


#include "graphics/textures/renderer.h"
#include "graphics/drawing/theme.h"
#include "graphics/drawing/scenes/bgcolor_scene.h"
#include "graphics/drawing/actions/bgcolor_action.h"
#include "graphics/resources/bgcolor_resource.h"

namespace graphics {
namespace drawing {
namespace scenes {

BGColorScene::BGColorScene(const std::string &name, const std::string &resource)
        : Scene(name, resource)
        , m_color(0.5f, 0.5f, 0.5f, 1.0f)
{
}

void BGColorScene::draw(graphics::textures::Renderer &renderer)
{
    auto *resource = parent()->resource(resourceId());
    auto *bgResource = dynamic_cast<resources::BGColorResource *>(resource);
    assert(bgResource != nullptr);

    bgResource->color() = m_color;
}

void BGColorScene::update(GLfloat dt)
{
    if (m_currentActionId.empty()) {
        return;
    }
    auto *action = dynamic_cast<actions::BGColorAction *>(m_actions[m_currentActionId]);


    dt *= 1000; // the domain here is milliseconds in stead of seconds
    action->update(dt);


    if (action->targetReached()) {
        action->resetTime();
        m_currentActionId = action->next();
        dynamic_cast<actions::BGColorAction *>(m_actions[m_currentActionId])->origin(m_color);
    }
}

} // namespace scenes
} // namespace drawing
} // namespace graphics