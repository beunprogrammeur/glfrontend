//
// Created by vincent on 19. 4. 21.
//

#include "graphics/drawing/actions/bgcolor_action.h"
#include "graphics/drawing/scenes/bgcolor_scene.h"

namespace graphics {
namespace drawing {
namespace actions {

BGColorAction::BGColorAction()
    : Action()
    , m_destination(0.0f, 0.0f, 0.0f, 1.0f)
    , m_origin(0.0f, 0.0f, 0.0f, 1.0f)
{
}


void BGColorAction::update(GLfloat dt)
{
    elapseTime(dt);

    auto formula = m_formula;
    if (targetReached()) {
        // prevent overshoot
        m_formula = UpdateFormula::Teleport;
    }
    auto* p = dynamic_cast<graphics::drawing::scenes::BGColorScene*>(parent());
    assert(p != nullptr);

    auto& target = p->color();

    applyFormula(target.r, m_destination.r, m_origin.r);
    applyFormula(target.g, m_destination.g, m_origin.g);
    applyFormula(target.b, m_destination.b, m_origin.b);

    m_formula = formula;
}

} // namespace actions
} // namespace drawing
} // namespace actions