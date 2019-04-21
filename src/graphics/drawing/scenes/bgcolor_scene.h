//
// Created by vincent on 19. 4. 21.
//

#ifndef GLFRONTEND_BGCOLOR_SCENE_H
#define GLFRONTEND_BGCOLOR_SCENE_H

#include <graphics/textures/renderer.h>
#include "graphics/drawing/scenes/scene.h"
#include "graphics/textures/texture2d.h"
#include "graphics/resources/bgcolor_resource.h"

namespace graphics {
namespace drawing {
namespace scenes {

class BGColorScene : public Scene
{
    glm::vec4 m_color;
public:
    BGColorScene(const std::string& name, const std::string& resource);

    void draw(graphics::textures::Renderer &renderer, graphics::resources::BGColorResource &resource);
    void update(GLfloat dt);
};

} // namespace scenes
} // namespace drawing
} // namespace graphics


#endif //GLFRONTEND_BGCOLOR_SCENE_H
