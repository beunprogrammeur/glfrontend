#ifndef GRAPHICS_DRAWING_ACTION_H
#define GRAPHICS_DRAWING_ACTION_H

#include <string>
#include <glad/glad.h>
#include <graphics/textures/renderer.h>


namespace graphics {

namespace textures {
class Renderer;
}

namespace drawing {

namespace scenes {
class Scene;
}

namespace actions {


class Action
{
public:
    enum class UpdateFormula
    {
        Teleport,
        Linear
    };

private:
    int m_elapsedTime;
    int m_totalTime;
    std::string m_next; // id if the next action
    scenes::Scene *m_parent;
protected:

    UpdateFormula m_formula;
    std::string m_id;

    template<typename T>
    void applyFormula(T &target, T dest, T origin, T ignoreValue = T{}, bool useIgnore = false);

    template<typename T>
    void linearApplyFormula(T &target, T dest, T origin, float totalTime, float elapsedTime);

    inline void elapseTime(GLfloat dt)
    { m_elapsedTime += static_cast<int>(dt); }

    inline scenes::Scene *parent() const
    { return m_parent; }

public:
    Action();

    inline void parent(scenes::Scene *parent)
    { m_parent = parent; }

    inline const std::string &next() const
    { return m_next; }

    inline void next(const std::string &next)
    { m_next = next; }

    inline void duration(GLfloat dt)
    { m_totalTime = static_cast<int>(dt); }

    inline bool targetReached() const
    { return m_elapsedTime >= m_totalTime; }

    inline void resetTime()
    { m_elapsedTime = 0; }

    inline UpdateFormula formula() const
    { return m_formula; }

    inline void formula(UpdateFormula value)
    { m_formula = value; }

    inline void id(const std::string &id)
    { m_id = id; }

    inline const std::string& id() const
    { return m_id; }

    inline virtual void update(GLfloat dt)
    {};

    inline virtual void draw(graphics::textures::Renderer &renderer)
    {};
};


template<typename T>
void Action::applyFormula(T &target, T dest, T origin, T ignoreValue, bool useIgnore)
{
    if (useIgnore && dest == ignoreValue) {
        return;
    }

    switch (m_formula) {
        case Action::UpdateFormula::Linear:
            linearApplyFormula(target, dest, origin, m_totalTime, m_elapsedTime);
            break;
        case Action::UpdateFormula::Teleport:
        default: // fall through
            target = dest;
            break;
    }
}

template<typename T>
void Action::linearApplyFormula(T &target, T dest, T origin, float totalTime, float elapsedTime)
{
    float delta = 1.0f / totalTime * elapsedTime;
    target = origin + ((dest - origin) * delta);
}

} // namespace actions
} // namespace drawing 
} // namespace graphics

#endif