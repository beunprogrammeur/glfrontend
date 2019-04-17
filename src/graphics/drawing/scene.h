#ifndef GRAPHICS_DRAWING_SCENE_H
#define GRAPHICS_DRAWING_SCENE_H
#include <string>
#include <map>

#include "graphics/drawing/action.h"
#include "graphics/textures/renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/resources/texture_resource.h"

namespace graphics
{
namespace drawing
{
class Scene 
{
private: 
    std::string m_resource;
    std::string m_name;
    std::map<std::string, Action> m_actions;
    std::string m_currentActionId;
    glm::vec2 m_position;
    glm::vec2 m_size;
    GLfloat m_opacity;
    GLfloat m_angle;
public: 
    Scene(const std::string& name, const std::string& resource);
    
    void draw(graphics::textures::Renderer& renderer, graphics::resources::TextureResource& resource);
    void update(GLfloat dt);

    void addAction(const std::string& id, Action action);
    inline const std::string& resourceId() const { return m_resource; }
};

} // namespace drawing
} // namespace graphics

#endif