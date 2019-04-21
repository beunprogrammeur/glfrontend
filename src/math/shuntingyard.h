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

bool calculateInfix(const std::string &expression, float& result, float hundredPercentValue = 100.0f, const std::map<std::string, float> &variables = {});

} // namespace shuntingyard
} // namespace math

#endif //GLFRONTEND_SHUNTINGYARD_H
