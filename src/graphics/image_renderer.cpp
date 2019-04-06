#include "image_renderer.h"

namespace graphics
{

ImageRenderer::ImageRenderer(/* args */)
    : m_program()
    , m_translate(glm::mat4(1.0f))
    , m_scale(glm::mat4(1.0f))
    , m_rotate(glm::mat4(1.0f))
{
}

ImageRenderer::~ImageRenderer()
{
}


void ImageRenderer::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    m_program.load(vertexShaderPath, fragmentShaderPath); 
	m_program.use();
    
    // initialize uniform variables using the uniform altering functions 
    // with default values
    translate();
    rotateDegrees();
    scale();
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

/*!
*  Translates the image(s) to be drawn with x and y on the screen
*  if Translate is called without arguments, the default 0.0f will be used (no translation)
*  this will reset the translation to put images back in the middle.
*/
void ImageRenderer::translate(float x, float y)
{
    m_translate = glm::mat4(1.0f);                    //x, y, z
    m_translate = glm::translate(m_translate, glm::vec3(x, y, 0.0f));

    m_program.use();
    m_program.setMat4("translate", m_translate);
}

/*!
*  Rotates the to be drawn image(s) with the given angle in degrees
*  If no argument is given the angle will be reset to 0
*  WARNING: does not account for aspect ratio. images skew if screen ratio is not 1:1
*/
void ImageRenderer::rotateDegrees(float angle)
{
    m_rotate = glm::mat4(1.0f);
    m_rotate = glm::rotate(m_rotate, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

    m_program.use();
    m_program.setMat4("rotate", m_rotate);
}

void ImageRenderer::scale(float factor)
{
    m_scale = glm::mat4(1.0f);
    m_scale = glm::scale(m_scale, glm::vec3(factor));

    m_program.use();
    m_program.setMat4("scale", m_scale);
}


} // namespace graphics


