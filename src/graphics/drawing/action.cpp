#include "graphics/drawing/action.h"

#include <bits/stdc++.h>

namespace graphics
{
namespace drawing
{ 

Action::Action()
    : m_waitMs(0)
    , m_actionDuration(0)
    , m_targetX(INT_MIN)
    , m_targetY(INT_MIN)
    , m_targetDegrees(0)
    , m_targetOpacity(1)
    , m_targetWidth(INT_MIN)
    , m_targetHeight(INT_MIN)
    , m_next("")
{  
}

} // namespace drawing
} // namespace graphics