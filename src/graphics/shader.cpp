#include "shader.h"
#include <iostream>

namespace graphics
{

Shader::Shader()
    : m_id(0)
{
}

void Shader::use()
{
    glUseProgram(m_id);
}

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
    GLuint sVertex   = 0;
    GLuint sFragment = 0;
    GLuint gShader   = 0;

    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    // if geomtetry source is available, compile that one too
    if(geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    // Shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, sVertex);
    glAttachShader(m_id, sFragment);
    if(geometrySource != nullptr)
    {
        glAttachShader(m_id, sVertex);
    }

    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    
    if(geometrySource!= nullptr)
    {
        glDeleteShader(gShader);
    }
}

void Shader::set(const GLchar* name, GLfloat value, GLboolean useShader)
{
    if (useShader) use();
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::set(const GLchar* name, GLint   value, GLboolean useShader)
{
    if (useShader) use();
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::set(const GLchar* name, const glm::vec2& value, GLboolean useShader)
{
    set(name, value.x, value.y, useShader);
}

void Shader::set(const GLchar* name, const glm::vec3& value, GLboolean useShader)
{
    set(name, value.x, value.y, value.z, useShader);
}

void Shader::set(const GLchar* name, const glm::vec4& value, GLboolean useShader)
{
    set(name, value.x, value.y, value.z, value.w, useShader);
}

void Shader::set(const GLchar* name, const glm::mat4& value, GLboolean useShader)
{
    if (useShader) use();
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader) use();
    glUniform2f(glGetUniformLocation(m_id, name), x, y);

}

void Shader::set(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader) use();
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);

}

void Shader::set(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader) use();
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success = 0;
    GLchar infoLog[1024];
    std::string specificMessage;

    if(type.compare("PROGRAM") == 0)
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, sizeof(infoLog), NULL, infoLog);
            specificMessage = "| ERROR:Shader: Compile-time error: Type: ";
        }    
    }
    else
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, sizeof(infoLog), NULL, infoLog);
            specificMessage = "| ERROR:Shader: Link-time error: Type: ";
        }
    }
    
    if(!success)
    {
        std::cout << specificMessage << type << std::endl 
                  << infoLog << std::endl
                  << "-- --------------------------------------- --" << std::endl;

    }
}

} // namespace graphics