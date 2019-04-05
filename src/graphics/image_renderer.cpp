#include "image_renderer.h"

namespace graphics
{

ImageRenderer::ImageRenderer(/* args */)
    : m_program()
{
}

ImageRenderer::~ImageRenderer()
{
}


void ImageRenderer::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    m_program.load(vertexShaderPath, fragmentShaderPath); 
	m_program.use();
    

    // WIRE FRAME (optional)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ImageRenderer::draw(Image& image)
{
    m_program.use();

    image.activate(Image::Layer::Layer0); 
    m_program.setInt("texture0", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


} // namespace graphics


