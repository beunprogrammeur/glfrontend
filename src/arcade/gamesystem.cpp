#include "gamesystem.h"

#include <dirent.h>
#include <stdlib.h>

#include <iostream>
#include <string.h>
#include <sstream>

#include "arcade/settings.h"
#include "filesystem/file.h"
#include "filesystem/path.h"
#include "filesystem/directory.h"
namespace arcade
{

GameSystem::GameSystem(const std::string path)
    : m_ini(Ini::fromFile(filesystem::path::concat(path, "config.ini")))
    , m_path(path)
    , m_games()
    , m_texture()
    , m_loaded()
    , m_selectedIndex()
{
    //Only support for PNG files for now
    filesystem::file::openTexture(filesystem::path::concat(path, "logo.png"), m_texture);
}

GameSystem::~GameSystem()
{
    // clear games if any
    m_ini.save(filesystem::path::concat(m_path, "config.ini"));
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

void GameSystem::loadGames()
{
    if(m_loaded)
    {
        return;
    }

    std::cout << "loading games for: " << friendlyName() << std::endl;

    std::vector<std::string> files;
    filesystem::file::getSubFiles(romPath(), files);

    for(const auto& file: files)
    {
        m_games.push_back(Game(file));
    }
    std::cout << "loaded " << m_games.size() << " games" << std::endl;
}

void GameSystem::runSelectedGame()
{
    if(m_selectedIndex > m_games.size() -1)
    {
        std::cerr << "error: " << __FUNCTION__ << ", selected index of game out of range" << std::endl;
        return;
    }

    // TODO: PCRE2 implementation? search and replace variables
    

    std::cout << "booting rom." << std::endl
              << "system: " << friendlyName() << std::endl
              << "rom:    " << m_games[m_selectedIndex].filename() << std::endl;


    if(emulatorPath().length() == 0)
    {
        std::cout << "emulator for " << friendlyName() << " not given."
                  << "see " << filesystem::path::concat(m_path, "config.ini") << std::endl;
        return;
    }


    std::stringstream ss;
    ss << emulatorPath();
    ss << " ";
    ss << filesystem::path::concat(romPath(), filesystem::path::escape(m_games[m_selectedIndex].filename()));
    std::cout << "command: " << ss.str() << std::endl;
    system(ss.str().c_str());
}

void GameSystem::selectNextGame()
{
    m_selectedIndex = (m_selectedIndex + 1) % m_games.size();
}

void GameSystem::selectPreviousGame()
{
    m_selectedIndex = (m_selectedIndex - 1) % m_games.size();
}





} // namespace arcade