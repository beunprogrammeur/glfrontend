//
// Created by vincent on 19. 4. 20.
//

#include "shuntingyard.h"
#include <sstream>
#include "debug/logger.h"
#include <cctype>

namespace math {
namespace shuntingyard {

// private
namespace {

char operators[]{'+', '-', '*', '/'};

inline bool isOperator(char c)
{
    for (char op : operators) {
        if (op == c) {
            return true;
        }
    }
    return false;
}

} // private namespace


bool calculateInfix(const std::string &expression, float& result, float hundredPercentValue, const std::map<std::string, float> &variables)
{
    //TODO: implement shunting yard algorithm so we can use complex calculations in theme positioning

    std::stringstream ss;
    debug::Logger log;
    log.print(expression);

    for (const auto c : expression) {
        if (isOperator(c))
        {
            log.print(c, " is an operator");
        }
        else if(c == ' ')
        {
            log.print("ignoring whitespace");
        }
        else if(std::isdigit(c))
        {
            log.print(c, " is a digit");
        }
        else if (std::isalpha(c))
        {
            log.print(c, " is a letter");
        }
        else if(c == '(')
        {
            log.warn(c, " is an opening bracket");
        }
        else if(c == ')')
        {
            log.warn(c, " is a closing bracket");
        }
        else
        {
            log.error(c, " is unkown");
        }
    }
}


} // namespace shuntingyard
} // namespace math
