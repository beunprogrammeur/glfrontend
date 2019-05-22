//
// Created by vincent on 19. 5. 22.
//

#include "filesystem/database/entity/game_system.h"

filesystem::database::entity::GameSystem::GameSystem(int id, const std::string &name, const std::string &friendly_name,
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
