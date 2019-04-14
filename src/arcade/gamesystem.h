#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include <ini/ini.h>
#include <string>
#include <vector>

#include "arcade/game.h"

#include "graphics/textures/texture2d.h"
#include "graphics/drawing/drawable.h"

#include "filesystem/path.h"

namespace arcade
{

class GameSystem : public graphics::drawing::Drawable
{
private:
    Ini m_ini;
    std::string m_path;
    std::vector<arcade::Game*> m_games;
    graphics::textures::Texture2D m_texture;
    bool m_loaded;
    GLint m_selectedIndex;
    std::string m_texturePath;

protected:
    std::string emulatorPath() { return m_ini.get("emulation", "emu_path"); };
    std::string romPath() { return m_ini.get("emulation",    "rom_dir", filesystem::path::concat(m_path, "games")); }
    std::string imgPath() { return m_ini.get("presentation", "img_dir", filesystem::path::concat(m_path, "wheel")); }

public:
    GameSystem(const std::string path);
    ~GameSystem();

    /*-- drawable --*/

    graphics::textures::Texture2D& texture() { return m_texture; }

    const std::string& texturePath() const { return m_texturePath; } 
    void texturePath(const std::string& path) { m_texturePath; }

    /*-- GameSystem --*/


    inline std::string friendlyName() { return m_ini.get("system","friendly_name"); };
    inline const std::vector<arcade::Game*>& games() const { return m_games; }
    
    // when selected
    void loadGames();
    // when unselected (lower memory usage)
    void clearGames(); 

    int index() const { return m_selectedIndex; }

    int runSelectedGame();
    void next();
    void prev();
    

    static  void loadSystems(std::vector<GameSystem*>& systems);
};

} // namespace arcade

#endif