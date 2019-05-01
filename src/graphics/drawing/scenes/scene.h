#ifndef GRAPHICS_DRAWING_SCENE_H
#define GRAPHICS_DRAWING_SCENE_H

#include <string>
#include <map>

#include "graphics/drawing/actions/action.h"

#include <glm/glm.hpp>


namespace graphics {
namespace drawing {

class Theme;

namespace scenes {

class Scene
{
private:
    std::string m_resource;
    std::string m_name;

protected:
    std::map<std::string, actions::Action *> m_actions;
    std::string m_currentActionId;
    Theme *m_parent;

public:
    Scene(const std::string &name, const std::string &resource);

    ~Scene();

    virtual void update(GLfloat dt) = 0;

    virtual void addAction(const std::string &id, actions::Action *action);

    inline const std::string &currentActionId()
    { return m_currentActionId; }

    inline const std::string &resourceId() const
    { return m_resource; }

    inline Theme *parent()
    { return m_parent; }

    inline void parent(Theme *parent)
    { m_parent = parent; }
};

} // namespace scenes
} // namespace drawing
} // namespace graphics

#endif