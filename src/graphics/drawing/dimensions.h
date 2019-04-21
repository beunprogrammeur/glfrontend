//
// Created by vincent on 19. 4. 19.
//

#ifndef GLFRONTEND_DIMENSIONS_H
#define GLFRONTEND_DIMENSIONS_H
#include <glad/glad.h>

#include <glm/glm.hpp>

namespace graphics
{
namespace drawing
{

struct Dimensions {

public:
    static constexpr GLfloat kPositionDefault = INT_MIN;
    static constexpr GLfloat kOpacityDefault = 1.0f;
    static constexpr GLfloat kAngleDefault = 0.0f;
    static constexpr GLfloat kSizeDefault = INT_MIN;
    static constexpr GLfloat kMarginDefault = 0.0f;


    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 margin;
    GLfloat angle;
    GLfloat opacity;


    Dimensions();


};


} // namespace drawing
} // namespace graphics


#endif //GLFRONTEND_DIMENSIONS_H
