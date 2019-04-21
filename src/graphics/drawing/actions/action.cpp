#include "graphics/drawing/actions/action.h"

#include <bits/stdc++.h>

namespace graphics {
namespace drawing {
namespace actions {

Action::Action()
        : m_elapsedTime(0)
          , m_totalTime(0)
          , m_next("")
          , m_formula(UpdateFormula::Teleport)
{
}

} // namespace actions
} // namespace drawing
} // namespace graphics