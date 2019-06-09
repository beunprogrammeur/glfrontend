//
// Created by vincent on 19. 4. 20.
//

#include "shuntingyard.h"
#include <sstream>
#include "debug/logger.h"
#include <cctype>
#include <vector>
#include <functional>

namespace math {
namespace shuntingyard {

std::map<char, int> Calculator::kOperators = {{'+', 1},
                                              {'-', 1},
                                              {'*', 2},
                                              {'/', 2}};


Calculator::Calculator()
        : m_variables()
          , m_debug("shunting yard calculator")
          , m_hundredPercent(100.0f)
{
}

void Calculator::setVariable(const std::string &name, float value)
{
    m_variables[name] = value;
}


void Calculator::setHundredPercent(float value)
{
    if (value == 0.0f) {
        m_debug.warn("trying to set hundred percent value to 0, ignoring. currently: ", m_hundredPercent, "%");
        return;
    }
    m_hundredPercent = value;
}

float Calculator::calculate(const std::string &formula, float defaultValue)
{
    if(formula.empty())
    {
        return defaultValue;
    }

    std::vector<char> ops;
    std::vector<float> values;

    const char *c = formula.c_str();
    for (; c[0] != '\0'; c++) {

        // ingore white spaces
        if (c[0] == ' ') {
            continue;
        }

        // brackets are seen as a special operator
        if (c[0] == '(') {
            ops.push_back(c[0]);
            continue;
        }


        // if it's a number, concat all the consecutive numbers for a number might be longer than one digit
        if (std::isdigit(c[0])) {
            std::stringstream ss;

            while (std::isdigit(c[0]) || c[0] == '.') {
                ss << c[0];
                c++;
            }

            float num = 1.0f;
            ss >> num;
            std::string str;
            ss >> str;

            // if the next character is a %, treat this substring as the percentage variable
            if (c[0] == '%') {
                c++;
                num = m_hundredPercent / 100.0f * num;
            }

            values.push_back(num);
            c--; // compensate for the last c++;
            continue;
        }

        // operate all grouped variables and operators between this bracket and the last one
        if (c[0] == ')') {
            while (!ops.empty() && ops.back() != '(') {
                float a = values.back();
                values.pop_back();

                float b = values.back();
                values.pop_back();

                char op = ops.back();
                ops.pop_back();

                values.push_back(operate(op, b, a));
            }
            ops.pop_back();
            continue;
        }

        // replace variable with number
        if (std::isalpha(c[0])) {
            std::stringstream ss;
            while (std::isalpha(c[0]) || c[0] == '_' || c[0] == '.') {
                ss << c[0];
                c++;
            }

            float variable = 0.0f;
            if (getVariable(ss.str(), variable)) {
                values.push_back(variable);
            }
            else {
                m_debug.warn("unidentified variable ", ss.str(), " in formula '", formula, "'");
                return defaultValue;
            }

            c--; // compensate for the last c++
            continue;
        }

        // execute all operators as long as possible
        if (isOperator(c[0])) {
            while (!ops.empty() && precedence(ops.back()) >= precedence(c[0])) {
                float a = values.back();
                values.pop_back();

                float b = values.back();
                values.pop_back();

                char op = ops.back();
                ops.pop_back();

                values.push_back(operate(op, b, a));
            }
            ops.push_back(c[0]);
            continue;
        }

        m_debug.error("unknown character '", c, "' in formula '", formula, "'");
        return defaultValue;
    }

    // all is parsed, now execute the remaining variables
    while (!ops.empty()) {
        float a = values.back();
        values.pop_back();

        float b = values.back();
        values.pop_back();

        char op = ops.back();
        ops.pop_back();

        values.push_back(operate(op, b, a));
    }

    return values.back();
}

bool Calculator::getVariable(const std::string &name, float &value)
{
    if (m_variables.find(name) != m_variables.end()) {
        value = m_variables[name];
        return true;
    }
    return false;
}

float Calculator::operate(const char op, const float left, const float right)
{
    switch (op) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            return left / right;
        default:
            return 0.0f;
    }
}

bool Calculator::isOperator(char op) const
{
    return kOperators.find(op) != kOperators.end();
}

int Calculator::precedence(char op) const
{

    return kOperators[op];
}

void Calculator::setHundredPercentByVariableName(const std::string &name)
{
    float value = 1.0f;
    if(getVariable(name, value))
    {
        setHundredPercent(value);
    }
}


} // namespace shuntingyard
} // namespace math
