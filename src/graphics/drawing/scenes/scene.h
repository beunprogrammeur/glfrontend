#ifndef GRAPHICS_DRAWING_SCENE_H
#define GRAPHICS_DRAWING_SCENE_H

#include <string>
#include <map>

#include "graphics/drawing/actions/action.h"

#include <glm/glm.hpp>


namespace graphics {


namespace textures {
class Renderer;
}

namespace drawing {

class Theme;


namespace scenes {

class Scene
{
private:
    std::string m_resource;
    std::string m_name;
    bool m_died;
    bool m_isDying;

protected:
    std::map<std::string, actions::Action *> m_actions;
    std::string m_currentActionId;
    Theme *m_parent;
    debug::Logger m_debug;
    /*
     * end state initiate for "kill" actions
     * */
    void die()
    { m_died = true; }

    void dieIfPlanned();

public:
    Scene(const std::string &name, const std::string &resource);

    virtual ~Scene();

    virtual void update(GLfloat dt) = 0;

    virtual void draw(graphics::textures::Renderer &renderer)
    {};

    virtual void addAction(actions::Action *action);

    inline const std::string &currentActionId() const
    { return m_currentActionId; }

    inline const std::string &resourceId() const
    { return m_resource; }

    inline Theme *parent()
    { return m_parent; }

    inline void parent(Theme *parent)
    { m_parent = parent; }

    /*
     * sets the "kill" scene action active if available
     * if the kill action is available, sets isDying = true, died = false;
     * if the kill action is not availale sets isDying = true, died = true;
     * */
    bool kill();

    /*
     * died is set after kill() (with no kill action) and when the kill action sequence has ended
     * */
    inline bool died()
    { return m_died; }

    inline bool isDying()
    { return m_isDying; }

    void verifyIds();
};

} // namespace scenes
} // namespace drawing
} // namespace graphics

#endif








