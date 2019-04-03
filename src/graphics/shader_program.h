#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics
{

class ShaderProgram
{
private:
    unsigned int m_shaderProgramId;

public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ShaderProgram();
    ~ShaderProgram();

    
    void load(const char* vertexPath, const char* fragmentPath);
    void use();

    void setBool(const  std::string& name, bool value) const;
	void setInt(const   std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const  std::string& name, glm::mat4 matrix) const;
};

} // namespace graphics

#endif