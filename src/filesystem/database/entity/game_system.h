//
// Created by vincent on 19. 5. 22.
//

#ifndef GLFRONTEND_GAME_SYSTEM_H
#define GLFRONTEND_GAME_SYSTEM_H

#include "filesystem/database/entity/db_entity.h"
#include <string>

namespace filesystem {
namespace database {
namespace entity {

class GameSystem : public DBEntity
{
private:
    std::string m_name;
    std::string m_friendlyName;
    std::string m_romPath;
    std::string m_imgPath;
    std::string m_vidPath;
    bool m_hasLogo;

public:
    GameSystem();
    GameSystem(int id, const std::string &name, const std::string &friendly_name, const std::string &rom_path,
               const std::string &img_path, const std::string &vid_path, bool has_logo);

    inline const std::string &name() const
    { return m_name; }

    inline const std::string &friendlyName() const
    { return m_friendlyName; }

    inline const std::string &romPath() const
    { return m_romPath; }

    inline const std::string &imgPath() const
    { return m_imgPath; }

    inline const std::string &vidPath() const
    { return m_vidPath; }

    inline bool hasLogo() const
    { return m_hasLogo; }

    GameSystem& operator+(const GameSystem& other);
};

} // namespace entity
} // namespace database
} // namespace filesystem

#endif //GLFRONTEND_GAME_SYSTEM_H
