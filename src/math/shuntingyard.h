//
// Created by vincent on 19. 4. 20.
//

#ifndef GLFRONTEND_SHUNTINGYARD_H
#define GLFRONTEND_SHUNTINGYARD_H

#include <string>
#include <map>
#include "debug/logger.h"

namespace math {

namespace shuntingyard {

class Calculator
{
private:
    std::map<std::string, float> m_variables;
    debug::Logger m_debug;
    float m_hundredPercent;

    static std::map<char, int> kOperators;


    bool getVariable(const std::string &name, float &value);
    static float operate(char op, float left, float right);
    bool isOperator(char op) const;
    int precedence(char op) const;
public:
    Calculator();

    void setHundredPercent(float value);
    void setHundredPercentByVariableName(const std::string& name);
    void setVariable(const std::string &name, float value);
    float calculate(const std::string &formula, float defaultValue = 0.0f);
};


} // namespace shuntingyard
} // namespace math

#endif //GLFRONTEND_SHUNTINGYARD_H
