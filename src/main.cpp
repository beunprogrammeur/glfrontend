#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem/database.h>

#include "graphics/engine.h"
#include "arcade/settings.h"
#include "debug/logger.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



graphics::Engine* cabinetPtr;
int main(int argc, char** argv)
{
	arcade::settings::init();
    cabinetPtr = nullptr;

    if(arcade::settings::debug::useColor())
    {
        debug::Logger::useColor();
    }


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = nullptr;
    if(arcade::settings::screen::isFullScreen())
    {
        window = glfwCreateWindow(arcade::settings::screen::width(), arcade::settings::screen::height(), "GLFrontend", glfwGetPrimaryMonitor(), nullptr);
        
    }
    else
    {
        window = glfwCreateWindow(arcade::settings::screen::width(), arcade::settings::screen::height(), "GLFrontend", nullptr, nullptr);
    }

    glfwMakeContextCurrent(window);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
    	std::cerr << "Failed to initialize GLAD" << std::endl;
    	return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glViewport(0,0, arcade::settings::screen::width(), arcade::settings::screen::height());
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    filesystem::database::init();
	graphics::Engine cabinet(arcade::settings::screen::width(), arcade::settings::screen::height());

    cabinet.init();
    cabinetPtr = &cabinet;

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    cabinet.state(graphics::Engine::State::Running);

    while(!glfwWindowShouldClose(window))
    {
        cabinet.handleState();
        if(cabinet.paused())
        {
            // don't draw when paused. handle state will wait 
            // for the child process to die. (emulator)
            continue;
        }

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        cabinet.processInputs(deltaTime);
        cabinet.update(deltaTime);

		cabinet.draw();

        glfwSwapBuffers(window);
    }

	arcade::settings::save();
    glfwDestroyWindow(window);
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // for now, only accept presses (no holds or releases)
    if(action != GLFW_PRESS)
    {
        return;
    }

    if(cabinetPtr)
    {
        switch(key)
        {
            case GLFW_KEY_UP:
                cabinetPtr->fireButton("up");
                break;
            case GLFW_KEY_DOWN:
                cabinetPtr->fireButton("down");
                break;
            case GLFW_KEY_ENTER:
            case GLFW_KEY_KP_ENTER:
                cabinetPtr->fireButton("select");
                break;
            case GLFW_KEY_BACKSPACE:
                cabinetPtr->fireButton("back");
                break;
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
            default:
                // unregistered key
                break;
        }
    }
}