//
// Created by vincent on 19. 5. 1.
//

#include <graphics/drawing/actions/wheel_action.h>
#include "graphics/drawing/theme.h"
#include "graphics/drawing/scenes/wheel_scene.h"
#include "graphics/drawing/scenes/drawable_scene.h"

namespace graphics {
namespace drawing {
namespace scenes {

WheelScene::WheelScene(const std::string &name, const std::string &resource)
        : Scene(name, resource)
          , m_selectedPosition()
          , m_drawingPosition()
          , m_transitionDispositionTarget(0.0f, 0.0f)
          , m_transitionDispositionCurrent(0.0f, 0.0f)
          , m_internalDrawingIndex(0)
          , m_indexInitialized(false)
{
}

void WheelScene::update(GLfloat dt)
{
    if (!m_indexInitialized) {
        m_indexInitialized = true;
        m_internalDrawingIndex = parent()->getWheelIndex();
    }
    // TODO: fix m_transitionDisposition variable settings (so we have an actual smooth transition)

    if (m_currentActionId.empty()) {
        return;
    }

    dt *= 1000; // the domain here is milliseconds in stead of seconds


    auto *action = dynamic_cast<actions::DrawableAction *>(m_actions[m_currentActionId]);
    action->update(dt);

    if (m_actions[actions::WheelAction::kID]) {
        m_actions[actions::WheelAction::kID]->update(dt);
    }


    if (action->targetReached()) {
        action->resetTime();
        m_currentActionId = action->next();
        dynamic_cast<actions::DrawableAction *>(m_actions[m_currentActionId])->origin(m_selectedPosition);
    }
}

void WheelScene::draw(graphics::textures::Renderer &renderer)
{
    parent()->drawWheel(
            renderer,
            m_selectedPosition,
            m_transitionDispositionCurrent,
            parent()->getDrawables(),
            m_internalDrawingIndex);

}

void WheelScene::addAction(actions::Action *action)
{
    std::string id = action->id();

    // Wheelactions are special. They never appear in the succession list.
    // They work parallel to the normal actions.
    // Therefore a WheelAction can never be the current action
    if (action->id() == actions::WheelAction::kID) {
        action->parent(this);
        m_actions[action->id()] = action;
        return;
    }

    if (currentActionId().empty()) {
        auto *drawableAction = dynamic_cast<actions::DrawableAction *>(action);
        if (drawableAction != nullptr) {
            m_selectedPosition = drawableAction->dimensions();
            drawableAction->origin(drawableAction->dimensions());
        }
    }

    Scene::addAction(action);
}


} // namespace scenes
} // namespace drawing
} // namespace graphics
