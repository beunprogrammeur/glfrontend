#include "arcade/gamesystemmanger.h"
#include "arcade/gamesystem.h"

#include <iostream>

namespace arcade
{

GameSystemManager::GameSystemManager()
    : m_active(nullptr)
    , m_systems()
    , m_index(0)
{
}

GameSystemManager::~GameSystemManager()
{
    m_active = nullptr;
    for(auto ptr : m_systems)
    {
        delete ptr;
    }
}

void GameSystemManager::load()
{
    GameSystem::loadSystems(m_systems);

    for(auto system : m_systems)
    {
        std::cout << "loaded system: " << system->friendlyName() << std::endl;
    }
}

void GameSystemManager::select()
{
    m_active = m_systems[m_index];
    m_active->loadGames();
}

void GameSystemManager::unselect()
{
    if(m_active != nullptr)
    {
        m_active->clearGames();
    }
    m_active = nullptr;
}

} // namespace arcade