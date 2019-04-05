#include "image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../arcade/cabinet.h"
#include <algorithm>

namespace graphics
{

Image::Image()
	: m_texture()
{
}

Image::~Image()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

Image::Image(const char* path)
	: m_texture()
{
	load(path);
}

void Image::load(const char* path)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);


	m_texture.load(path);

	int screenw = arcade::Cabinet::kScreenWidth;
	int screenh = arcade::Cabinet::kScreenHeight;
	int imgw = m_texture.width();
	int imgh = m_texture.height();
	float screenFactor = (float)screenw / (float)screenh;
	float imgFactor = (float)imgw / (float)imgh;

	// image is more landscape than the screen
	if(imgFactor > screenFactor)
	{
		double resizeFactor = (double)imgw / (double)screenw;
		screenw *= resizeFactor;
		screenh *= resizeFactor;
	}
	else
	{
		// screen is more portrait thant he image
		double resizeFactor = (double)imgh / (double)screenh;
		screenw *= resizeFactor;
		screenh *= resizeFactor;
	}
	
	// in OpenGL the screen is -1 by 1 in both x and y
	// take the scale in base of a dimension -1 to 0 and 0 to 1
	// calculate how much scaled size difference is between the image/screen bounds
	float xoffset = 1.0f -(1.0f / screenw * imgw);
	float yoffset = 1.0f -(1.0f / screenh * imgh);

	// apply the xy offset on the "normal" value of 1's. (outer bounds / fill screen)
	float vertices[] = {   
        // positions             // texture coords
		 1.0f - xoffset,  1.0f - yoffset, 0.0f,     1.0f, 1.0f, // top right
		 1.0f - xoffset, -1.0f + yoffset, 0.0f,     1.0f, 0.0f, // bottom right
		-1.0f + xoffset, -1.0f + yoffset, 0.0f,     0.0f, 0.0f, // bottom left
		-1.0f + xoffset,  1.0f - yoffset, 0.0f,     0.0f, 1.0f  // top left
	};
	
	// OpenGL draws rectangles, using 2 rectangles we make a triangle
	// the triangles have duplicate vertices
	// we use indices to not send duplicates, but to tell GL to use some mutliple times.
	// this is kind of overkill for 2 rectangles, but it scales well and is a proper way.
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	glBindVertexArray(m_vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // Bind a certain buffer type to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vao);
		
	
	// set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBindVertexArray(m_vao);

	m_texture.setActive();
}

} /** namespace graphics **/
