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
#include "filesystem/database/database.h"

namespace arcade
{
debug::Logger GameSystem::s_debug("gamesystem");

GameSystem::GameSystem(const std::string path)
    : m_ini(Ini::fromFile(filesystem::path::concat(path, "config.ini")))
    , m_path(path)
    , m_games()
    , m_texture()
    , m_loaded(false)
    , m_selectedIndex(0)
    , m_texturePath(filesystem::path::concat(path, "logo.png"))
{
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

    filesystem::directory::getSubDirs(root, [&](const char* dir){
        auto* system = new GameSystem(filesystem::path::concat(root, dir));

        filesystem::database::update(*system);

        systems.emplace_back(system);
    });
}

void GameSystem::loadGames()
{
    if(m_loaded)
    {
        return;
    }

    s_debug.print("loading games for: ", friendlyName());

    std::vector<std::string> files;
    filesystem::file::getSubFiles(romPath(), files);

    std::string texPath = imgPath();
    for(const auto& file: files)
    {
        auto g = new Game(file, filesystem::path::concat(texPath, filesystem::path::changeExtension(file, ".png"))); 
        filesystem::file::openTexture(filesystem::path::concat(imgPath(), file), g->texture());
        m_games.push_back(g);
    }
    s_debug.print("loaded ", m_games.size()," games");
}

void GameSystem::clearGames()
{
    for(auto ptr : m_games)
    {
        delete ptr;
    }
    m_games.clear();
}

/* Runs the by this gamesystem selected game
*  With the given emulator (if available)
*  Returns: the pid of the child process that runs the game
*/
int GameSystem::runSelectedGame()
{
    if(m_games.empty())
    {
        return -1;
    }

    if(m_selectedIndex > m_games.size() -1)
    {
        s_debug.error("selected index(", m_selectedIndex, ") out of range");
        return -1;
    }

    // TODO: PCRE2 implementation? search and replace variables
    
    s_debug.print("booting rom: ", m_games[m_selectedIndex]->filename(), " for system: ", friendlyName());


    if(emulatorPath().length() == 0)
    {
        s_debug.warn("emulator not configured for ", friendlyName(), "\n",
                     "see ", filesystem::path::concat(m_path, "config.ini"));
        return -1;
    }

    namespace fs = filesystem;
    
    return fs::file::execute(emulatorPath(), fs::path::concat(romPath(), m_games[m_selectedIndex]->filename()), true, true);
}

void GameSystem::next()
{
    if(!m_games.empty())
    {
        m_selectedIndex++;
        if(m_selectedIndex >= m_games.size())
        {
            m_selectedIndex = 0;
        }
    }
}

void GameSystem::prev()
{
    if(!m_games.empty())
    {
        m_selectedIndex--;
        if(m_selectedIndex < 0)
        {
            m_selectedIndex = m_games.size() - 1;
        } 
    }
}





} // namespace arcade