#include "graphics/drawing/scenes/scene.h"
#include <iostream>

namespace graphics {
namespace drawing {
namespace scenes {

Scene::Scene(const std::string &name, const std::string &resource)
        : m_resource(resource)
          , m_name(name)
          , m_died(false)
          , m_isDying(false)
          , m_actions()
          , m_currentActionId("")
          , m_debug("scene '" + name + "'")
{

}

void Scene::addAction(actions::Action *action)
{
    m_actions[action->id()] = action;
    action->parent(this);
    if (m_currentActionId.empty()) {
        m_currentActionId = action->id();
    }
}

Scene::~Scene()
{
    for (auto pair : m_actions) {
        delete pair.second;
    }
}

bool Scene::kill()
{
    if (m_actions.find("kill") != m_actions.end()) {
        m_currentActionId = "kill";
        m_isDying = true;
        return true;
    }
    m_died = true;
    m_isDying = true;
    return false;
}

void Scene::dieIfPlanned()
{
    if (isDying() && m_currentActionId == m_actions[m_currentActionId]->next()) {
        die();
    }
}

void Scene::verifyIds()
{
    for (auto action : m_actions) {
        auto act = action.second;
        if (m_actions.find(act->next()) == m_actions.end()) {
            m_debug.warn("action '", act->id(), "' references to action '", act->next(),
                         "' but is not found. auto self reference added to '", act->id(), "'");
            act->next(act->id());
        }
    }
}


} // namespace scenes
} // namespace drawing 
} // namespace graphics