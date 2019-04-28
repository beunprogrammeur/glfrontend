//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_DRAWABLE_ACTION_H
#define GLFRONTEND_DRAWABLE_ACTION_H

#include "graphics/drawing/actions/action.h"
#include "graphics/drawing/dimensions.h"

namespace graphics {
namespace drawing {
namespace actions {

class DrawableAction : public Action
{
private:
    graphics::drawing::Dimensions m_dimensions;
    graphics::drawing::Dimensions m_origin;

public:
    DrawableAction();

    inline void origin(Dimensions dimensions)
    { m_origin = dimensions; }

    inline Dimensions dimensions()
    { return m_dimensions; }

    inline void dimensions(Dimensions dimensions)
    { m_dimensions = dimensions; }

    void update(Dimensions &target, GLfloat dt);
};

} // namespace actions
} // namespace drawing
} // namespace graphics

#endif //GLFRONTEND_DRAWABLE_ACTION_H
