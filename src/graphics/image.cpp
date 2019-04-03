#include "image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace graphics
{

Image::Image()
	: m_texture()
{
}

Image::~Image()
{
}

Image::Image(const char* path)
	: m_texture(path)
{
}

void Image::load(const char* path)
{
	m_texture.load(path);
}

void Image::activate(Image::Layer layer)
{
	switch(layer)
	{
		case Image::Layer::Layer0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case Image::Layer::Layer1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case Image::Layer::Layer2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case Image::Layer::Layer3:
			glActiveTexture(GL_TEXTURE3);
			break;
		default:
			return;
	}

	m_texture.setActive();
}

} /** namespace graphics **/
