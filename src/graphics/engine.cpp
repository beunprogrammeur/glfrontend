#include "graphics/engine.h"
#include "arcade/settings.h"
#include "filesystem/file.h"
#include "filesystem/directory.h"
#include "arcade/settings.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <sys/wait.h>

namespace graphics {

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
          , m_bgColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
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
    auto& dimensions = m_wheel.dimensions();
    dimensions.size.x = 200;
    dimensions.size.y = 100;
    dimensions.margin.x = 10;
    dimensions.margin.y = 10;
    dimensions.position.x = arcade::settings::screen::width() - (dimensions.size.x / 2.0f) - dimensions.margin.x;
    dimensions.position.y = arcade::settings::screen::height() - (dimensions.size.y / 2.0f) - dimensions.margin.y - ((dimensions.size.y + dimensions.margin.y) *2);

    m_systemManager.load();


    m_theme.load("./data/theme.json", m_bgColor);


    initInputs();
}

void Engine::initInputs()
{
    m_buttons["up"] = input::Button([&]() {
        if (m_systemManager.hasSelected()) {
            m_systemManager.activeSystem()->next();
        }
        else {
            m_systemManager.next();
        }
    });

    m_buttons["down"] = input::Button([&]() {
        if (m_systemManager.hasSelected()) {
            m_systemManager.activeSystem()->prev();
        }
        else {
            m_systemManager.prev();
        }
    });

    m_buttons["select"] = input::Button([&]() {
        if (m_systemManager.hasSelected()) {
            pid_t pid = m_systemManager.activeSystem()->runSelectedGame();
            if (pid > 0) {
                m_gamePid = pid;
                pause();
            }
        }
        else {
            m_systemManager.select();
            std::cout << "selected" << m_systemManager.activeSystem()->friendlyName() << std::endl;
        }
    });

    m_buttons["back"] = input::Button([&]() {
        if (m_systemManager.hasSelected()) {
            m_systemManager.unselect();
        }
    });
}

void Engine::draw()
{
    glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_theme.draw(*m_renderer);



    if (m_systemManager.hasSelected()) {
        // draw game wheel
        m_wheel.draw(*m_renderer, m_systemManager.activeSystem()->games(), m_systemManager.activeSystem()->index());
    }
    else {
        // draw system wheel
        m_wheel.draw(*m_renderer, m_systemManager.systems(), m_systemManager.index());
    }
}

void Engine::update(GLfloat dt)
{
    m_theme.update(dt, m_bgColor, m_wheel.dimensions());
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

void Engine::handleState()
{
    if (paused() && m_gamePid > 0) {
        int status = 0;
        if (waitpid(m_gamePid, &status, WNOHANG) == 0) {
            // game is running
            std::chrono::milliseconds ms(250);
            std::this_thread::sleep_for(ms);
        }
        else {
            // signaling failed. program has quit.
            m_gamePid = 0;
            resume();
        }
    }
}

} // namespace graphics