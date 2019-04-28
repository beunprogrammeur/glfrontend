//
// Created by vincent on 19. 4. 21.
//

#include "drawable_action.h"

namespace graphics {
namespace drawing {
namespace actions {

DrawableAction::DrawableAction()
    : Action()
    , m_dimensions()
    , m_origin()
{
    m_dimensions.angle = 0.0f;
    m_dimensions.opacity = 1.0f;

    m_origin.angle = 0.0f;
    m_origin.opacity = 1.0f;
}

void DrawableAction::update(Dimensions &target, GLfloat dt)
{
    elapseTime(dt);

    auto formula = m_formula;
    if (targetReached()) {
        // prevent overshoot
        m_formula = UpdateFormula::Teleport;
    }

    applyFormula(target.angle, m_dimensions.angle, m_origin.angle);
    applyFormula(target.opacity, m_dimensions.opacity, m_origin.opacity);
    applyFormula(target.position.x, m_dimensions.position.x, m_origin.position.x, Dimensions::kPositionDefault, true);
    applyFormula(target.position.y, m_dimensions.position.y, m_origin.position.y, Dimensions::kPositionDefault, true);
    applyFormula(target.size.x, m_dimensions.size.x, m_origin.size.x, Dimensions::kSizeDefault, true);
    applyFormula(target.size.y, m_dimensions.size.y, m_origin.size.y, Dimensions::kSizeDefault, true);
    applyFormula(target.displacement.x, m_dimensions.displacement.x, m_origin.displacement.x, Dimensions::kDisplacementDefault, true);
    applyFormula(target.displacement.y, m_dimensions.displacement.y, m_origin.displacement.y, Dimensions::kDisplacementDefault, true);

    m_formula = formula;
}

} // namespace actions
} // namespace drawing
} // namespace graphics