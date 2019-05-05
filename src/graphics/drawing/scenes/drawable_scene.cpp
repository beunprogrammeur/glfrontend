//
// Created by vincent on 19. 4. 21.
//

#include "graphics/drawing/actions/drawable_action.h"
#include "graphics/drawing/wheel.h"
#include "graphics/resources/wheel_resource.h"
#include "drawable_scene.h"
#include "graphics/drawing/theme.h"

namespace graphics {
namespace drawing {
namespace scenes {

DrawableScene::DrawableScene(const std::string &name, const std::string &resource)
        : Scene(name, resource)
          , m_dimensions()
{
    m_dimensions.opacity = 1.0f;
    m_dimensions.angle = 0.0f;
}

void DrawableScene::draw(graphics::textures::Renderer &renderer)
{
    auto *resource = dynamic_cast<resources::DrawableResource *>(parent()->resource(resourceId()));


    renderer.draw(resource->texture(),
                  m_dimensions.position,
                  m_dimensions.size,
                  m_dimensions.angle,
                  glm::vec3(1.0f, 1.0f, 1.0f), m_dimensions.opacity);
}

void DrawableScene::update(GLfloat dt)
{
    if (m_currentActionId.empty()) {
        return;
    }
    auto *action = dynamic_cast<actions::DrawableAction *>(m_actions[m_currentActionId]);


    dt *= 1000; // the domain here is milliseconds in stead of seconds
    action->update(dt);


    if (action->targetReached()) {
        action->resetTime();
        m_currentActionId = action->next();
        dynamic_cast<actions::DrawableAction *>(m_actions[m_currentActionId])->origin(m_dimensions);
    }
}

void DrawableScene::addAction(actions::Action *action)
{
    if (currentActionId().empty()) {
        this->m_dimensions = dynamic_cast<actions::DrawableAction *>(action)->dimensions();
    }
    Scene::addAction(action);
}

} // namespace scenes
} // namespace drawing
} // namespace graphics
