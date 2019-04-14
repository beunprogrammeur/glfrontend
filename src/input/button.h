#ifndef INPUT_BUTTON_H
#define INPUT_BUTTON_H
#include <functional>

namespace input
{

class Button
{
private:
    std::function<void()> m_event;

public:
    Button();
    Button(std::function<void()> event);

    void bind(std::function<void()> event);
    void fire() const;
};

} // namespace input

#endif