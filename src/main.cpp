#include <iostream>
#include "arcade/cabinet.h"

int main(const int argc, const char** argv)
{
	std::cout << "GL frontend" << std::endl;
	
	arcade::Cabinet cabinet;
	if(!cabinet.init(argc, argv))
	{
		std::cout << "error: initialization of OpenGL/cabinet failed" << std::endl;
	}
	
	cabinet.draw();
	
	return 0;
}
