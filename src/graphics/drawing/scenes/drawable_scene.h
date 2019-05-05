//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_DRAWABLE_SCENE_H
#define GLFRONTEND_DRAWABLE_SCENE_H

#include "graphics/drawing/scenes/scene.h"
#include "graphics/textures/renderer.h"

#include "graphics/resources/texture_resource.h"
#include "graphics/drawing/dimensions.h"

namespace graphics {

// forward declare
namespace resource {
class Resource;
}

namespace drawing {
namespace scenes {

class DrawableScene : public Scene
{
private:
    graphics::drawing::Dimensions m_dimensions;
public:
    DrawableScene(const std::string &name, const std::string &resource);

    void draw(graphics::textures::Renderer &renderer) override;

    void update(GLfloat dt) override;

    void addAction(actions::Action *action) override;

    graphics::drawing::Dimensions &dimensions()
    { return m_dimensions; }
};

} // namespace scenes
} // namespace drawing
} // namespace graphics

#endif //GLFRONTEND_DRAWABLE_SCENE_H
