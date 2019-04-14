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

    std::string texPath = imgPath();
    for(const auto& file: files)
    {
        auto g = new Game(file, filesystem::path::concat(texPath, filesystem::path::changeExtension(file, ".png"))); 
        filesystem::file::openTexture(filesystem::path::concat(imgPath(), file), g->texture());
        m_games.push_back(g);
    }
    std::cout << "loaded " << m_games.size() << " games" << std::endl;
}

void GameSystem::clearGames()
{
    for(auto ptr : m_games)
    {
        delete ptr;
    }
    m_games.clear();
}

int GameSystem::runSelectedGame()
{
    if(m_games.empty())
    {
        return -1;
    }

    if(m_selectedIndex > m_games.size() -1)
    {
        std::cerr << "error: " << __FUNCTION__ 
                  << ", selected index(" 
                  << m_selectedIndex 
                  << ") of game out of range" << std::endl;
        return -1;
    }

    // TODO: PCRE2 implementation? search and replace variables
    

    std::cout << "booting rom." << std::endl
              << "system: "     << friendlyName() << std::endl
              << "rom:    "     << m_games[m_selectedIndex]->filename() << std::endl;


    if(emulatorPath().length() == 0)
    {
        std::cout << "emulator for " << friendlyName() << " not given."
                  << "see "          << filesystem::path::concat(m_path, "config.ini") << std::endl;
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