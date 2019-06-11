#include "debug/logger.h"

namespace debug {

const std::string Logger::mk_COLOR_RED_BOLD = "\033[1;31m";
const std::string Logger::mk_COLOR_BLUE_BOLD = "\033[1;34m";
const std::string Logger::mk_COLOR_YELLOW_BOLD = "\033[1;33m";
const std::string Logger::mk_COLOR_RESET = "\033[0m";
bool Logger::mk_useColor = false;

void Logger::useColor()
{
    mk_useColor = true;
}


void Logger::printHead()
{
    if (mk_useColor) {
        std::cout << mk_COLOR_BLUE_BOLD;
        std::cout << m_name << ' ';
    }
    else {
        std::cout << m_name << " notice: ";
    }

    if (mk_useColor) {
        std::cout << mk_COLOR_RESET;
    }
}

void Logger::errorHead()
{
    if (mk_useColor) {
        std::cout << mk_COLOR_RED_BOLD;
        std::cout << m_name << ' ';
    }
    else {
        std::cout << m_name << " error: ";
    }

    if (mk_useColor) {
        std::cout << mk_COLOR_RESET;
    }
}


void Logger::warnHead()
{
    if (mk_useColor) {
        std::cout << mk_COLOR_YELLOW_BOLD;
        std::cout << m_name << ' ';
    }
    else {
        std::cout << m_name << " warning: ";
    }

    if (mk_useColor) {
        std::cout << mk_COLOR_RESET;
    }
}

} // namespace debug