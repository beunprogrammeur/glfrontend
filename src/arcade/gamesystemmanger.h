#ifndef ARCADE_GAMESYSTEMMANAGER_H
#define ARCADE_GAMESYSTEMMANAGER_H

#include <vector>
#include <string>
#include "debug/logger.h"

namespace arcade
{

class GameSystem;

class GameSystemManager
{
private:
    GameSystem* m_active;
    std::vector<GameSystem*> m_systems;

    int m_index;

    static debug::Logger s_debug;

public:
     GameSystemManager();
    ~GameSystemManager();

    void load();

    inline void next()     { m_index  = (++m_index >=    m_systems.size() ? 0 : m_index); }
    inline void prev()     { m_index  = (--m_index < 0 ? m_systems.size() -1 : m_index);  }
    
    void select();              
    void unselect();
    inline bool hasSelected()   const { return m_active != nullptr;    }
    inline int  index()         const { return m_index;                }
    
    inline GameSystem* activeSystem() const { return m_active;               }
    inline std::vector<GameSystem*>& systems() { return m_systems; }
};

} // namespace arcade

#endif