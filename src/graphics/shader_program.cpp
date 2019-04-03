#include "shader_program.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <fstream>
#include <sstream> 

namespace graphics
{

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
    : m_shaderProgramId(0)
{
    load(vertexPath, fragmentPath);
}

ShaderProgram::ShaderProgram()
    : m_shaderProgramId(0)
{
}

void ShaderProgram::load(const char* vertexPath, const char* fragmentPath)
{

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    std::stringstream vertexStream;
    std::stringstream fragmentStream;


    vertexFile.exceptions   (std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        vertexStream   << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

    }
    catch(std::ifstream::failure e)
    {
        std::cout << "error: failed to load shader: " << e.what() << std::endl;
        return;
    }

    std::string vstring = vertexStream.str();
    std::string fstring = fragmentStream.str();

    const GLchar* vertexSource   = vstring.c_str();
    const GLchar* fragmentSource = fstring.c_str();
    int vlength = vstring.length();
    int flength = fstring.length();


    uint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, &vlength);
    glCompileShader(vertex);

    uint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, &flength);
    glCompileShader(fragment);

    int success = 0;
    char log[512] = {0};

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, sizeof(log), nullptr, log);
        std::cout << "error: failed to compile vertex shader source: " << log << std::endl;
        return;
    }

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, sizeof(log), nullptr, log);
        std::cout << "error: failed to compile fragment shader source: " << log << std::endl;
        return;
    }

    m_shaderProgramId = glCreateProgram();
    glAttachShader(m_shaderProgramId, vertex);
    glAttachShader(m_shaderProgramId, fragment);
    glLinkProgram(m_shaderProgramId);

    glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_shaderProgramId, sizeof(log), nullptr, log);
        std::cout << "error: failed to link shader program: " << log << std::endl;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

ShaderProgram::~ShaderProgram()
{
    glUseProgram(0);
    glDeleteProgram(m_shaderProgramId);
}

void ShaderProgram::use()
{
    glUseProgram(m_shaderProgramId);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramId, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgramId, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_shaderProgramId, name.c_str()), value);
}

void ShaderProgram::setMat4(const  std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace graphics