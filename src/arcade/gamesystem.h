#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include <ini/ini.h>
#include <string>
#include <vector>

#include "graphics/textures/texture2d.h"

namespace arcade
{

class GameSystem
{
private:
    Ini m_ini;
    std::string m_path;
    graphics::textures::Texture2D m_texture;


public:
    GameSystem(const std::string path);
    ~GameSystem();

    graphics::textures::Texture2D& texture() { return m_texture; }

    static  void loadSystems(std::vector<GameSystem*>& systems);
    inline std::string friendlyName() { return m_ini.get("system","friendly_name"); };

};

} // namespace arcade

#endif