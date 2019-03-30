#ifndef CABINET_H
#define CABINET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace arcade
{

class Cabinet
{
public:
	Cabinet();
	~Cabinet();
		
	bool init(const int argc, const char* argv[]);
		
	virtual void draw();
		
protected:
	virtual bool initGL(const int argc, const char* argv[]);
	virtual bool initCabinet(const int argc, const char* argv[]);
	
	virtual void handleInput();
private:
	GLFWwindow* m_window;
};

} /** namespace arcade **/
#endif
