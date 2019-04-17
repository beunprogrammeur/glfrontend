#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <string>
#include <iostream>

namespace debug
{
class Logger
{
private:
    std::string m_name;

    static bool mk_useColor;

    template<typename T>
    void privPrint(T message);

    template<typename T, typename... Messages>
    void privPrint(T message, Messages... messages);

    static const std::string mk_COLOR_RED_BOLD;
    static const std::string mk_COLOR_BLUE_BOLD;
    static const std::string mk_COLOR_YELLOW_BOLD;
    static const std::string mk_COLOR_RESET;
    void printHead();
    void errorHead();
    void warnHead();

public:
    Logger(const std::string& name) : m_name(name) {}
    Logger() : m_name("") {}

    void name(std::string& name) { m_name = name; }
    const std::string& name()    { return m_name; }

    template<typename T>
    void print(T message);

    template<typename T, typename... Messages>
    void print(T message, Messages... messages);

    template<typename T>
    void error(T message);

    template<typename T, typename... Messages>
    void error(T message, Messages... messages);

    template<typename T>
    void warn(T message);

    template<typename T, typename... Messages>
    void warn(T message, Messages... messages);


    static void useColor();
};

template<typename T>
void Logger::print(T message)
{
    printHead();
    privPrint(message);
    std::cout << std::endl;
}

template<typename T, typename... Messages>
void Logger::print(T message, Messages... messages)
{
    printHead();
    privPrint(message, messages...);
    std::cout << std::endl;
}

template<typename T>
void Logger::error(T message)
{
    errorHead();
    privPrint(message);
    std::cout << std::endl;
}

template<typename T, typename... Messages>
void Logger::error(T message, Messages... messages)
{
    errorHead();
    privPrint(message, messages...);
    std::cout << std::endl;
}

template<typename T>
void Logger::warn(T message)
{
    warnHead();
    privPrint(message);
    std::cout << std::endl;
}

template<typename T, typename... Messages>
void Logger::warn(T message, Messages... messages)
{
    warnHead();
    privPrint(message, messages...);
    std::cout << std::endl;
}

template<typename T>
void Logger::privPrint(T message)
{
    std::cout << message;
}

template<typename T, typename... Messages>
void Logger::privPrint(T message, Messages... messages)
{
    privPrint(message);
    privPrint(messages...);
}


} // namespace debug


#endif