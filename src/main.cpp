#include <iostream>
#include "arcade/cabinet.h"
#include "arcade/settings.h"

int main(const int argc, const char** argv)
{
	std::cout << "GL frontend" << std::endl;
	
	arcade::settings::init();
	arcade::Cabinet cabinet;
	if(!cabinet.init(argc, argv))
	{
		std::cout << "error: initialization of OpenGL/cabinet failed" << std::endl;
	}
	
	cabinet.draw();
	


	arcade::settings::save();
	return 0;
}
