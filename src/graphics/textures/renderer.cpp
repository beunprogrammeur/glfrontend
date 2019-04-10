#include "graphics/textures/renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "filesystem/file.h"
#include <iostream>

namespace graphics
{
namespace textures
{

Renderer::Renderer()
    : m_shader()
    , m_quadVAO(0)
{
    init();
}

Renderer::Renderer(graphics::Shader shader)
    : m_shader(shader)
    , m_quadVAO(0)
{
    init();
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void Renderer::init()
{
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Renderer::compile(const char* vertPath, const char* fragPath, const char* geoPath)
{
    std::string vertSrc;
    std::string fragSrc;    
    std::string geoSrc;

    if(vertPath == nullptr)
    {
        std::cerr << "error in: " << __FUNCTION__ << ", vertPath can not be nullptr" << std::endl;
    }

    if(vertPath == nullptr)
    {
        std::cerr << "error in: " << __FUNCTION__ << ", fragPath can not be nullptr" << std::endl;
    }

    if(!filesystem::file::readText(vertPath, vertSrc))
    {
        std::cerr << "error in: " << __FUNCTION__ << ", failed to read file: " << vertPath << std::endl;
    }
    
    if(!filesystem::file::readText(fragPath, fragSrc))
    {
        std::cerr << "error in: " << __FUNCTION__ << ", failed to read file: " << fragPath << std::endl;
    }    

    if(geoPath != nullptr && filesystem::file::readText(geoPath, geoSrc))
    {
        std::cerr << "error in: " << __FUNCTION__ << ", failed to read file: " << geoPath << std::endl;
    }

    m_shader.compile(vertSrc.c_str(), fragSrc.c_str(), geoPath != nullptr ? geoSrc.c_str() : nullptr); 
}

void Renderer::draw(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
    m_shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(   model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0f));
    model = glm::scale(    model, glm::vec3(size, 1.0f));

    m_shader.set("model", model);
    m_shader.set("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    // bind VAO since we're going to draw a rectangle
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // unbind to prevent usage of our VAO in other classes
    glBindVertexArray(0);
}

} // namespace textures
} // namespace graphics