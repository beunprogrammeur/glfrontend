#ifndef GRAPICS_TEXTURES_RENDERER_H
#define GRAPICS_TEXTURES_RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "graphics/shader.h"
#include "graphics/textures/texture2d.h"

namespace graphics
{
namespace textures
{
class Renderer
{
private:
    graphics::Shader m_shader;
    GLuint m_quadVAO; // VAO with information for drawing rectangles

    void init();

public:
    Renderer();
    Renderer(graphics::Shader shader);
    ~Renderer();

    inline graphics::Shader& shader() { return m_shader; }

    void compile(const char* vertPath, const char* fragPath, const char* geoPath = nullptr);
    void draw(graphics::textures::Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f), GLfloat opacity = 1.0f);
};


} // namespace textures
} // namespace graphics


#endif