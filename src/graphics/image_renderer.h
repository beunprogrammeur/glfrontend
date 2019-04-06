#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "image.h"
#include "shader_program.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{
    
class ImageRenderer
{
private:
    ShaderProgram m_program;
    glm::mat4 m_translate;
    glm::mat4 m_scale;
    glm::mat4 m_rotate;
    
public:
    ImageRenderer();
    ~ImageRenderer();

    void loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
    void draw(Image& image);
    void translate(float x = 0.0f, float y = 0.0f);
    void rotateDegrees(float angle = 0.0f);
    void scale(float factor = 1.0f);
};



} // graphics




#endif