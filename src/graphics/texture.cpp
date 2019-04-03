#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

namespace graphics
{

Texture::Texture()
	: m_textureId(0)
	, m_isLoaded(false)
{
}	

Texture::Texture(const char* path)
	: m_textureId(0)
	, m_isLoaded(false)
{
	load(path);
}

Texture::~Texture()
{
	unload();
}

void Texture::load(const char* path)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width = 0, height = 0, nChannels = 0;
	
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

	unsigned int colorFlag = GL_RGB;
	if(std::string(path).find(".png") != std::string::npos)
	{
		colorFlag = GL_RGBA;
	}

	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorFlag, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
	
	m_isLoaded = true;
}

void Texture::unload()
{
	if(isLoaded())
	{
		glDeleteTextures(1, &m_textureId);
	}

	m_textureId = 0;
}

void Texture::setActive()
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

	
} /** namespace graphics **/