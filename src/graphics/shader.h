#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

namespace graphics
{

class Shader
{
private:
    GLuint m_id;

public:
    Shader();
    void use();

    void compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr);

    void set(const GLchar* name, GLfloat value, GLboolean useShader = false);
    void set(const GLchar* name, GLint   value, GLboolean useShader = false);
    void set(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
    void set(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
    void set(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
    void set(const GLchar* name, const glm::mat4& value, GLboolean useShader = false);
    void set(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void set(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void set(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);

private:
    void checkCompileErrors(GLuint object, std::string type);


};  
} // namespace graphics
#endif