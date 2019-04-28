//
// Created by vincent on 19. 4. 20.
//

#ifndef GLFRONTEND_SHUNTINGYARD_H
#define GLFRONTEND_SHUNTINGYARD_H

#include <string>
#include <map>

namespace math
{

namespace shuntingyard
{

float calc(const std::string &expression, float hundredPercentValue,
           std::map<std::string, float> &variables);

} // namespace shuntingyard
} // namespace math

#endif //GLFRONTEND_SHUNTINGYARD_H
