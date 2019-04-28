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

// private
namespace {

std::map<char, int> operators = {{'+', 1},
                                 {'-', 1},
                                 {'*', 2},
                                 {'/', 2}};

std::map<char, std::function<float(float, float)>> operate = {
        {'+', [](float a, float b) { return a + b; }},
        {'-', [](float a, float b) { return a - b; }},
        {'*', [](float a, float b) { return a * b; }},
        {'/', [](float a, float b) { return a / b; }},
};

debug::Logger log("shunting yard");

inline bool isOperator(char c)
{
    return operators.find(c) != operators.end();
}

inline int precedence(char op)
{
    if(!isOperator(op))
    {
        return 0;
    }

    return operators[op];
}

} // private namespace


float calc(const std::string &expression, float hundredPercentValue,
          std::map<std::string, float> &variables)
{
    std::vector<char> ops;
    std::vector<float> values;

    const char *c = expression.c_str();
    for (; c[0] != '\0'; c++) {
        if (c[0] == ' ') {
            continue;
        }

        if (c[0] == '(') {
            ops.push_back(c[0]);
            continue;
        }


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

            if (c[0] == '%') {
                c++;
                num = hundredPercentValue / 100.0f * num;
            }

            values.push_back(num);
            c--; // compensate for the last c++;
            continue;
        }


        if (c[0] == ')') {
            while (!ops.empty() && ops.back() != '(') {
                float a = values.back();
                values.pop_back();

                float b = values.back();
                values.pop_back();

                char op = ops.back();
                ops.pop_back();

                values.push_back(operate[op](b, a));
            }
            ops.pop_back();
            continue;
        }

        if (std::isalpha(c[0])) {
            std::stringstream ss;
            while (std::isalpha(c[0])) {
                ss << c[0];
                c++;
            }

            if (variables.find(ss.str()) != variables.end()) {
                float variable = variables[ss.str()];
                values.push_back(variable);
            }
            else {
                log.warn("unidentified variable ", ss.str(), " in formula '", expression, "'");
            }

            c--; // compensate for the last c++
            continue;
        }

        if (isOperator(c[0])) {
            while (!ops.empty() && precedence(ops.back()) >= precedence(c[0]))
            {
                float a = values.back();
                values.pop_back();

                float b = values.back();
                values.pop_back();

                char op = ops.back();
                ops.pop_back();

                values.push_back(operate[op](b, a));
            }
            ops.push_back(c[0]);
            continue;
        }

        log.error("unknown character '", c, "' in formula '", expression, "'");
    }

    while(!ops.empty())
    {
        float a = values.back();
        values.pop_back();

        float b = values.back();
        values.pop_back();

        char op = ops.back();
        ops.pop_back();

        values.push_back(operate[op](b, a));
    }

    return values.back();
}


} // namespace shuntingyard
} // namespace math
