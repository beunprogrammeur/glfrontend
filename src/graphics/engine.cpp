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
    , m_systems()
    , m_gameRunning(false)
    , m_gamePid(0)
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

    arcade::GameSystem::loadSystems(m_systems);

    for(auto system : m_systems)
    {
        std::cout << "loaded system: " << system->friendlyName() << std::endl;
    }


    //m_systems[0]->loadGames();
    //m_systems[0]->selectNextGame();
    //int pid = m_systems[0]->runSelectedGame();
    //if(pid > 0)
    //{
    //    m_gameRunning = true;
    //    m_gamePid = pid;
    //    m_state = State::Paused;
    //}
}

void Engine::draw()
{
    // m_renderer->draw(*m_tex1, glm::vec2(150, 200), glm::vec2(300, 400), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    // m_renderer->draw(*m_tex2, glm::vec2(500, 200), glm::vec2(300, 400), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    int i = 1;
    for(auto system : m_systems)
    {
        m_renderer->draw(system->texture(), glm::vec2(100*i, 100*i), glm::vec2(100, 100));
        i++;
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
}

void Engine::resume()
{   
}

} // namespace graphics