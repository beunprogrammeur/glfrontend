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

void DrawableScene::draw(graphics::textures::Renderer &renderer, graphics::resources::Resource* resource)
{

    if(resource->type() == graphics::resources::Resource::Type::Wheel)
    {
        auto* resGame = dynamic_cast<graphics::resources::WheelResource<arcade::Game>*>(resource);
        auto* resGameSys = dynamic_cast<graphics::resources::WheelResource<arcade::GameSystem>*>(resource);

        int index = parent()->getWheelIndex();

        if(resGame != nullptr)
        {
            graphics::drawing::wheel::draw(renderer, m_dimensions, resGame->drawables(), index);
        }
        else if(resGameSys != nullptr)
        {
            graphics::drawing::wheel::draw(renderer, m_dimensions, resGameSys->drawables(), index);
        }
        else
        {
            // incompatible type
        }
    }
    else {
        auto* res = dynamic_cast<graphics::resources::DrawableResource*>(resource);
        renderer.draw(res->texture(), m_dimensions.position, m_dimensions.size, m_dimensions.angle,
                      glm::vec3(1.0f, 1.0f, 1.0f), m_dimensions.opacity);
    }
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

void DrawableScene::addAction(const std::string &id, actions::Action *action)
{
    if(currentActionId().empty())
    {
        this->m_dimensions = static_cast<actions::DrawableAction*>(action)->dimensions();
    }
    Scene::addAction(id, action);
}

} // namespace scenes
} // namespace drawing
} // namespace graphics
