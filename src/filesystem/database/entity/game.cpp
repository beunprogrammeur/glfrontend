//
// Created by vincent on 19. 5. 22.
//

#include "filesystem/database/entity/game.h"


namespace filesystem {
namespace database {
namespace entity {

Game::Game()
        : DBEntity(-1)
          , m_systemId(-1)
          , m_name("")
          , m_friendlyName("")
          , m_hasImg(false)
          , m_hasVid(false)
{}

Game::Game(int id, int systemId, const std::string &name, const std::string &friendly_name, bool hasImg, bool hasVid)
        : DBEntity(id)
          , m_systemId(systemId)
          , m_name(name)
          , m_friendlyName(friendly_name)
          , m_hasImg(hasImg)
          , m_hasVid(hasVid)
{
}


Game &Game::operator=(const Game &other)
{
    id(other.id());
    m_systemId = other.m_systemId;
    m_name = other.m_name;
    m_friendlyName = other.m_friendlyName;
    m_hasImg = other.m_hasImg;
    m_hasVid = other.m_hasVid;

    return *this;
}

} // namespace entity
} // namespace database
} // namespace filesystem

