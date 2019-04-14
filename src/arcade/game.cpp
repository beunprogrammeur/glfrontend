#include "arcade/game.h"
#include <regex>

namespace arcade
{

Game::Game(const std::string filename, const std::string texturePath)
    : m_texture()
    , m_friendlyName("")
    , m_filename(filename)
    , m_texturePath(texturePath)
{
    // ...
}

void Game::createFriendlyName()
{
    // TODO: regex friendly name
    // filter: _.[]! 
}


} // namespace arcade