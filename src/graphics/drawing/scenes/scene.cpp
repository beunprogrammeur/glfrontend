#include "graphics/drawing/scenes/scene.h"
#include <iostream>

namespace graphics {
namespace drawing {
namespace scenes {

Scene::Scene(const std::string &name, const std::string &resource)
        : m_resource(resource)
          , m_name(name)
          , m_actions()
          , m_currentActionId("")
{

}

void Scene::addAction(const std::string &id, actions::Action* action)
{
    m_actions[id] = action;
    action->parent(this);
    if (m_currentActionId.empty()) {
        m_currentActionId = id;
    }
}

Scene::~Scene()
{
    for(auto pair : m_actions) {
        delete pair.second;
    }
}


} // namespace scenes
} // namespace drawing 
} // namespace graphics