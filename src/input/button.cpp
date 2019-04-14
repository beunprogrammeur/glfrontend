#include "button.h"

namespace input
{

Button::Button()
    : m_event()
{}

Button::Button(std::function<void()> event)
    : m_event(event)
{
}

void Button::bind(std::function<void()> event)
{
    m_event = event;
}

void Button::fire() const
{
    if(m_event)
    {
        m_event();
    }
}

} // namespace input
