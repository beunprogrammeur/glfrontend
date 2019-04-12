#include "arcade/game.h"
#include <regex>

namespace arcade
{

Game::Game(const std::string filename)
    : m_texture()
    , m_friendlyName("")
    , m_filename(filename)
{
    // ...
}

void Game::createFriendlyName()
{
    // TODO: regex friendly name
    // filter: _.[]! 
}


} // namespace arcade