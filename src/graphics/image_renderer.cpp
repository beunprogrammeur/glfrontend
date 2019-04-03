#include "image_renderer.h"

namespace graphics
{

ImageRenderer::ImageRenderer(/* args */)
    : m_program()
{
}

ImageRenderer::~ImageRenderer()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}


void ImageRenderer::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    m_program.load(vertexShaderPath, fragmentShaderPath); 

    float vertices[] = {   
        //positions             //texture coords
		 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f
	};
	
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	// VertexBufferObject
	glGenBuffers(1, &m_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // Bind a certain buffer type to VBO
	
	// Put the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Element Buffer Object
	glGenBuffers(1, &m_ebo);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Vertex Array Object initialization and binding (VBO parent)
	glGenVertexArrays(1, &m_vao);
	
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vao);
		
	
	// set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    // WIRE FRAME (optional)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ImageRenderer::draw(Image& image)
{
    m_program.use();

    image.activate(Image::Layer::Layer0); 
    m_program.setInt("texture0", 0);

    glBindVertexArray(m_vao);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


} // namespace graphics


