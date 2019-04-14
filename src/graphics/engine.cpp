#include "graphics/engine.h"
#include "arcade/settings.h"
#include "filesystem/file.h"
#include "filesystem/directory.h"
#include "arcade/settings.h"

#include <iostream>

namespace graphics
{

Engine::Engine(const GLuint width, const GLuint height)
    : m_width(width)
    , m_height(height)
    , m_state(State::Running)
    , m_renderer()
    , m_systemManager()
    , m_gameRunning(false)
    , m_gamePid(0)
    , m_wheel()
    , m_buttons()
{
}

Engine::~Engine()
{
    delete m_renderer;
}


void Engine::init()
{
    m_renderer = new graphics::textures::Renderer();
    m_renderer->compile("data/shaders/sprite.vert", "data/shaders/sprite.frag", nullptr);
    
    //configuring the shaders
    glm::mat4 projection = glm::ortho(0.0f, 
        static_cast<GLfloat>(m_width),
        static_cast<GLfloat>(m_height), 0.0f, -1.0f, 1.0f);

    m_renderer->shader().use();
    m_renderer->shader().set("image", 0);
    m_renderer->shader().set("projection", projection);



    // these variables will be set by the theme manager in the future
    m_wheel.itemWidth(200);
    m_wheel.itemHeight(100);
    m_wheel.itemMargin(10);
    m_wheel.selectedItemPosX(arcade::settings::screen::width() - m_wheel.itemWidth());
    m_wheel.selectedItemPosY(arcade::settings::screen::height() / 2 - (m_wheel.itemHeight() / 2));

    m_systemManager.load();

    m_buttons["up"] = input::Button([&]() {
        if(m_systemManager.hasSelected())
        {
            m_systemManager.activeSystem()->next();
        }
        else
        {
            m_systemManager.next();
        }
    });

    m_buttons["down"] = input::Button([&]() {
        if(m_systemManager.hasSelected())
        {
            m_systemManager.activeSystem()->prev();
        }
        else
        {
            m_systemManager.prev();
        }
    });

    m_buttons["select"] = input::Button([&](){
        if(m_systemManager.hasSelected())
        {
            m_systemManager.activeSystem()->runSelectedGame();
        }
        else
        {
            m_systemManager.select();
            std::cout << "selected" << m_systemManager.activeSystem()->friendlyName() << std::endl;
        }
    });

    m_buttons["back"] = input::Button([&](){
        if(m_systemManager.hasSelected())
        {
            m_systemManager.unselect();
        }            
    });
/*
    m_systems[5]->loadGames();
    

    m_buttons["up"] = input::Button([&]() {
        if(m_activeGameSystem)
        {
            m_activeGameSystem->selectPreviousGame();    
        }
        else
        {

        }
        m_systems[5]->selectPreviousGame();
        std::cout << "system, index: " << m_systems[5]->friendlyName() << ", " << m_systems[5]->selectedIndex() << std::endl;
    });

    m_buttons["down"] = input::Button([&]() {
        m_systems[5]->selectNextGame();
    });
*/
}

void Engine::draw()
{
    if(m_systemManager.hasSelected())
    {
        // draw game wheel
        m_wheel.draw(*m_renderer, m_systemManager.activeSystem()->games(), m_systemManager.activeSystem()->index());
    }
    else
    {
        // draw system wheel

        m_wheel.draw(*m_renderer, m_systemManager.systems(), m_systemManager.index());
    }
    


}

void Engine::update(GLfloat dt)
{

}

void Engine::processInputs(GLfloat dt)
{
    
}

void Engine::pause()
{
    m_state = State::Paused;
}

void Engine::resume()
{   
    m_state = State::Running;
}

} // namespace graphics