//
// Created by vincent on 19. 5. 22.
//

#include "filesystem/database/entity/game_system.h"

namespace filesystem {
namespace database {
namespace entity {

GameSystem::GameSystem()
        : DBEntity(-1)
          , m_name("")
          , m_friendlyName("")
          , m_romPath("")
          , m_imgPath("")
          , m_vidPath("")
          , m_hasLogo(false)
{}

GameSystem::GameSystem(int id, const std::string &name, const std::string &friendly_name,
                       const std::string &rom_path, const std::string &img_path,
                       const std::string &vid_path, bool has_logo)
        : DBEntity(id)
          , m_name(name)
          , m_friendlyName(friendly_name)
          , m_romPath(rom_path)
          , m_imgPath(img_path)
          , m_vidPath(vid_path)
          , m_hasLogo(has_logo)
{
}


GameSystem &GameSystem::operator+(const GameSystem &other)
{
    id(other.id());
    m_name = other.m_name;
    m_friendlyName = other.m_friendlyName;
    m_romPath = other.m_romPath;
    m_imgPath = other.m_imgPath;
    m_vidPath = other.m_vidPath;
    m_hasLogo = other.m_hasLogo;
}

} // namespace entity
} // namespace database
} // namespace filesystem
