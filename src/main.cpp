#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/engine.h"
#include "arcade/settings.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main(int argc, char** argv)
{
	arcade::settings::init();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);   
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(arcade::settings::screen::width(), arcade::settings::screen::height(), "Breakout", nullptr, nullptr);
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

	graphics::Engine cabinet(arcade::settings::screen::width(), arcade::settings::screen::height());

    cabinet.init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    cabinet.state(graphics::Engine::State::Running);

    while(!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        cabinet.processInputs(deltaTime);
        cabinet.update(deltaTime);

        glClearColor(1.0f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
		cabinet.draw();

        glfwSwapBuffers(window);
    }

	arcade::settings::save();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ||
       key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } 
    /*
	if(key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
        {
            Breakout.Keys[key] = GL_TRUE;
        }
        else if(action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = GL_FALSE;
        }
        else
        {
            // nothing here..
        }  
    }
	*/
}