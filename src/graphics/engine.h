#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/textures/renderer.h"

#include "arcade/gamesystemmanger.h"
#include "arcade/gamesystem.h"

#include "input/button.h"

#include "graphics/drawing/theme.h"


namespace graphics {

class Engine
{
public:
    enum class State
    {
        Running,
        Paused
    };

private:
    GLuint m_width;
    GLuint m_height;
    State m_state;
    graphics::textures::Renderer *m_renderer;
    arcade::GameSystemManager m_systemManager;
    bool m_gameRunning;
    int m_gamePid;
    std::map<std::string, input::Button> m_buttons;
    graphics::drawing::Theme m_theme; // main wheel theme
    glm::vec4 m_bgColor;
    debug::Logger m_debug;
    int m_mainWheelIndex;

    void initInputs();

    void loadMainTheme();

    void loadSubTheme(filesystem::database::entity::GameSystem &system);

public:
    Engine(const GLuint width, const GLuint height);

    ~Engine();

    void init();

    void draw();

    void update(GLfloat dt);

    void processInputs(GLfloat dt);

    inline State state() const
    { return m_state; }

    inline void state(State state)
    { m_state = state; }

    void pause(); // for when the program loses focus
    bool paused() const
    { return m_state == State::Paused; }

    void resume();

    void handleState();

    void fireButton(const std::string &name)
    { m_buttons[name].fire(); }

};

} // namespace graphics

#endif