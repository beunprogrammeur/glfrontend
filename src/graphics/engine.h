#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/textures/renderer.h"
#include "arcade/gamesystem.h"

namespace graphics
{

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
    State  m_state;
    graphics::textures::Renderer* m_renderer;
    std::vector<arcade::GameSystem*> m_systems;
    bool m_gameRunning;
    int m_gamePid;

public:
    Engine(const GLuint width, const GLuint height);
    ~Engine();
    void init();
    void draw();
    void update(GLfloat dt);
    void processInputs(GLfloat dt);

    inline State state() const      { return m_state;  }
    inline void  state(State state) { m_state = state; }


    void pause(); // for when the program loses focus
    void resume(); 

    
};

} // namespace graphics

#endif