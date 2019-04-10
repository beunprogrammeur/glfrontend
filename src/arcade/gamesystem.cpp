#include "gamesystem.h"

#include <dirent.h>
#include "settings.h"
#include <iostream>
#include <string.h>
#include "filesystem/file.h"
#include "filesystem/directory.h"


namespace arcade
{

GameSystem::GameSystem(const std::string path)
    : m_ini(Ini::fromFile(filesystem::directory::concat(path, "config.ini")))
    , m_path(path)
    , m_texture()
{
    //Only support for PNG files for now
    filesystem::file::openTexture(filesystem::directory::concat(path, "logo.png"), m_texture);
}

GameSystem::~GameSystem()
{
    // clear games if any
    m_ini.save(filesystem::directory::concat(m_path, "config.ini"));
}

/*!
*   Create a list of preconfigured game systems based on their conifg.ini file on the hdd.
*/
void GameSystem::loadSystems(std::vector<GameSystem*>& systems)
{
    auto root = arcade::settings::gaming::gameSystemsRootDir();
    
    std::vector<std::string> directories;
    filesystem::directory::getSubDirs(arcade::settings::gaming::gameSystemsRootDir(), directories);
    for(auto path : directories)
    {
        systems.push_back(new GameSystem(path));
    }
}

} // namespace arcades