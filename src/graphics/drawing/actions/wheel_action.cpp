//
// Created by vincent on 19. 5. 1.
//

#include <graphics/drawing/scenes/wheel_scene.h>
#include "graphics/drawing/actions/wheel_action.h"
#include "graphics/drawing/theme.h"


namespace graphics {
namespace drawing {
namespace actions {

const std::string WheelAction::kID = "scroll";

WheelAction::WheelAction()
        : DrawableAction()
          , m_targetDisplacement(0.0f, 0.0f)
          , m_scrollTotalTime(0)
          , m_scrollCurrentTime(0)
{
    m_formula = Action::UpdateFormula::Linear;
}

void WheelAction::update(GLfloat dt)
{
    elapseTime(dt);


    auto *theme = parent()->parent();

    auto *scene = dynamic_cast<scenes::WheelScene *>(parent());

    if (targetReached() && !theme->wheelIndexChanged()) {
        scene->transitionDispositionCurrent(glm::vec2(0.0f, 0.0f));
        dynamic_cast<scenes::WheelScene *>(parent())->internalDrawingIndex(parent()->parent()->getWheelIndex());
        m_targetDisplacement.x = 0.0f;
        m_targetDisplacement.y = 0.0f;
        return;
    }
    auto dispos = dynamic_cast<scenes::WheelScene *>(parent())->transitionDispositionCurrent();

    if (theme->wheelIndexChanged()) {
        if (targetReached()) {
            resetTime();
        }
        elapseTime(-dt);

        m_targetDisplacement.x -= scene->dimensions().displacement.x * theme->indexDifferential();
        m_targetDisplacement.y -= scene->dimensions().displacement.y * theme->indexDifferential();

        dispos = m_targetDisplacement;
    }

    applyFormula(dispos.x, m_targetDisplacement.x, 0.0f);
    applyFormula(dispos.y, m_targetDisplacement.y, 0.0f);

    scene->transitionDispositionCurrent(dispos);
}

} // namespace actions
} // namespace drawing
} // namespace graphics
