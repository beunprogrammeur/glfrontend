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
namespace drawing {
namespace scenes {

class DrawableScene : public Scene
{
private:
    graphics::drawing::Dimensions m_dimensions;
public:
    DrawableScene(const std::string& name, const std::string& resource);
    
    void draw(graphics::textures::Renderer &renderer, graphics::resources::Resource* resource);
    void update(GLfloat dt);

    void addAction(const std::string& id, actions::Action* action) override;
};

} // namespace scenes
} // namespace drawing
} // namespace graphics

#endif //GLFRONTEND_DRAWABLE_SCENE_H
