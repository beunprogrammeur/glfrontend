//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_BGCOLOR_SCENE_H
#define GLFRONTEND_BGCOLOR_SCENE_H

#include "graphics/drawing/scenes/scene.h"

namespace graphics {

// forward declarations
namespace textures {
class Renderer;
}

namespace resources {
class BGColorResource;
}

namespace drawing {
namespace scenes {

class BGColorScene : public Scene
{
    glm::vec4 m_color{};
public:
    BGColorScene(const std::string &name, const std::string &resource);

    void draw(graphics::textures::Renderer &renderer) override;

    void update(GLfloat dt) override;

    inline glm::vec4 &color()
    { return m_color; }
};

} // namespace scenes
} // namespace drawing
} // namespace graphics


#endif //GLFRONTEND_BGCOLOR_SCENE_H
