#include "cabinet.h"

#include <iostream>
#include <string.h>
#include <string>



namespace arcade
{

int Cabinet::kScreenHeight = 0;
int Cabinet::kScreenWidth = 0;

Cabinet::Cabinet()
	: m_window(nullptr)
    , m_imageRenderer()
    , m_image()
{
}

Cabinet::~Cabinet()
{
	//glfwTerminate();
}


bool Cabinet::init(const int argc, const char* argv[])
{
	if(!initGL(argc, argv))
	{
		return false;
	}
	
	if(!initCabinet(argc, argv))
	{
		return false;
	}
	
	return true; 
} 

bool Cabinet::initGL(const int argc, const char* argv[])
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// making it fullscreen, this mechanic needs improvement
    if(argc >= 2 && strcmp(argv[1], "-fullscreen") == 0)
    {
        auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        m_window = glfwCreateWindow(mode->width, mode->height, WINDOW_TITLE, monitor, nullptr);
        kScreenWidth  = mode->width;
        kScreenHeight = mode->height;
    }
    else
    {
        m_window = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
        kScreenWidth  = DEFAULT_SCREEN_WIDTH;
        kScreenHeight = DEFAULT_SCREEN_HEIGHT;
    }
    
    if(m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(m_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
        // TODO: save screen size state in a file
    });
	
	return true;
}

bool Cabinet::initCabinet(const int argc, const char* argv[])
{
    m_imageRenderer.loadShaders(IMAGE_SHADER_VERTEX_FILE, IMAGE_SHADER_FRAGMENT_FILE);
    m_image.load("./data/media/ebichu.png");
    
	return true;
}

void Cabinet::handleInput()
{
	if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
	   glfwGetKey(m_window, GLFW_KEY_Q)      == GLFW_PRESS  )
	{
		glfwSetWindowShouldClose(m_window, true);
	} 
}

void Cabinet::draw()
{
    float deg = 0.0f;

    m_imageRenderer.scale(0.5);
    float x = 0.0f;
    float y = 0.0f;

	while(!glfwWindowShouldClose(m_window))
	{
		handleInput();
		glClearColor(0.7f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
        m_imageRenderer.rotateDegrees(deg+=0.1f);
        m_imageRenderer.translate(x+=0.0001, y+=0.0000);
        m_imageRenderer.draw(m_image);




		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}


} /** namespace arcade **/
