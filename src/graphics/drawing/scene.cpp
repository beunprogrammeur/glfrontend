#include "graphics/drawing/scene.h"
#include <iostream>
namespace graphics
{
namespace drawing
{
Scene::Scene(const std::string& name, const std::string& resource)
    : m_resource(resource)
    , m_name(name)
    , m_actions()
    , m_currentActionId("")
    , m_position(0,0)
    , m_size(0,0)
    , m_opacity(1.0f)
    , m_angle(0.0f)
{

}

void Scene::addAction(const std::string& id, Action action)
{
    m_actions[id] = action;
    if(m_currentActionId.empty())
    {
        m_currentActionId = id;
    }
}

void Scene::draw(graphics::textures::Renderer& renderer, graphics::resources::TextureResource& resource)
{
    renderer.draw(resource.texture(), m_position, m_size, m_angle, glm::vec3(1.0f, 1.0f, 1.0f), m_opacity);

}

void Scene::update(GLfloat dt)
{
    if(m_currentActionId.empty())
    {
        return;
    }
    Action& action = m_actions[m_currentActionId];
    

    dt*=1000; // the domain here is milliseconds in stead of seconds

    if(action.targetReached())
    {
        if(action.targetX() != INT_MIN)
        {
            m_position.x = action.targetX();
        }

        if(action.targetY() != INT_MIN)
        {
            m_position.y = action.targetY();
        }

        if(action.targetWidth() != INT_MIN)
        {
            m_size.x = action.targetWidth();
        }

        if(action.targetHeight() != INT_MIN)
        {
            m_size.y = action.targetHeight();
        }

        m_angle = action.targetDegrees();
        m_opacity = action.targetOpacity();
    }
    else
    {
        float factor = (1.0f / static_cast<float>(action.waitMs())); // dt is seconds. we want milliseconds
        
        if(action.targetX() != INT_MIN)
        {
            int difference = action.targetX() - m_position.x;
            m_position.x  += difference * factor;
        }

        if(action.targetY() != INT_MIN)
        {
            int difference = action.targetY() - m_position.y; 
            m_position.y  += difference * factor;
        }

        if(action.targetWidth() != INT_MIN)
        {
            int difference = action.targetWidth() - m_size.x; 
            m_size.x += difference * factor;
        }

        if(action.targetHeight() != INT_MIN)
        {
            int difference = action.targetHeight() - m_size.y;
            m_size.y += difference * factor;
        }
        float odifference = action.targetOpacity() - m_opacity;
        float adifference = action.targetDegrees() - m_angle;
        m_angle   += adifference * factor;
        m_opacity += odifference * factor;
    }
    


    if(action.targetReached())
    {
        action.resetTime();
        m_currentActionId = action.next();
    }
    else
    {
        action.ellapseTime(dt); // to milliseconds
    }
}

} // namespace drawing 
} // namespace graphics