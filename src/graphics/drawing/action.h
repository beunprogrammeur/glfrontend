#ifndef GRAPHICS_DRAWING_ACTION_H
#define GRAPHICS_DRAWING_ACTION_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace graphics
{ 
namespace drawing
{

class Action
{
private:
    int m_waitMs;  
    int m_actionDuration;    
    int m_targetX;  
    int m_targetY;  
    int m_targetDegrees;
    GLfloat m_targetOpacity;
    int m_targetWidth;
    int m_targetHeight;
    std::string m_next; // id if the next action

public:
    Action(); 

    inline const std::string& next() const    { return m_next; }
    inline void next(const std::string& next) { m_next = next; }

    inline int waitMs()  const { return m_waitMs;  }
    inline int targetX() const { return m_targetX; }
    inline int targetY() const { return m_targetY; }
    inline int targetDegrees() const { return m_targetDegrees; }
    inline GLfloat targetOpacity() const { return m_targetOpacity; }
    inline int targetWidth()   const { return m_targetWidth;   }
    inline int targetHeight()  const { return m_targetHeight;  }
 
    inline void waitMs(int target)  { m_waitMs = target;  }
    inline void targetX(int target) { m_targetX = target; }
    inline void targetY(int target) { m_targetY = target; }
    inline void targetDegrees(int target) { m_targetDegrees = target; }
    inline void targetOpacity(GLfloat target) { m_targetOpacity = target; }
    inline void targetWidth(int target)   { m_targetWidth = target;   }
    inline void targetHeight(int target)  { m_targetHeight = target;  }

    inline void actionDuration(int total) { m_actionDuration = total; }
    inline int  actionDuration() { return m_actionDuration; }
    inline void ellapseTime(GLfloat dt) { m_waitMs -= dt; }
    inline bool targetReached() const   { return m_waitMs <= 0; }
    inline void resetTime() { waitMs(actionDuration()); }

};


} // namespace drawing 
} // namespace graphics

#endif