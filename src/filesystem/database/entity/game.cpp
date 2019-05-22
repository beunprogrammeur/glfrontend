//
// Created by vincent on 19. 5. 22.
//

#include "filesystem/database/entity/game.h"


namespace filesystem {
namespace database {
namespace entity {

Game::Game(int id, int systemId, const std::string &name, const std::string &friendly_name, bool hasImg, bool hasVid)
    : DBEntity(id)
    , m_systemId(systemId)
    , m_name(name)
    , m_friendlyName(friendly_name)
    , m_hasImg(hasImg)
    , m_hasVid(hasVid)
{
}

} // namespace entity
} // namespace database
} // namespace filesystem

