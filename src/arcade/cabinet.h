#ifndef CABINET_H
#define CABINET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../graphics/image_renderer.h"
#include "../graphics/image.h"

namespace arcade
{

class Cabinet
{
private:
	GLFWwindow* m_window;
	graphics::ImageRenderer m_imageRenderer;
	graphics::Image m_image1;
	graphics::Image m_image2;

protected:
	virtual bool initGL(const int argc, const char* argv[]);
	virtual bool initCabinet(const int argc, const char* argv[]);
	
	virtual void handleInput();

public:
	Cabinet();
	~Cabinet();
		
	bool init(const int argc, const char* argv[]);
		
	virtual void draw();
};

} /** namespace arcade **/
#endif
