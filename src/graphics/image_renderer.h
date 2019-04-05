#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "image.h"
#include "shader_program.h"

namespace graphics
{
    
class ImageRenderer
{
private:
    ShaderProgram m_program;
    /* data */
public:
    ImageRenderer(/* args */);
    ~ImageRenderer();

    void loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
    void draw(Image& image);
};



} // graphics




#endif