#ifndef ARCADE_GAME_H
#define ARCADE_GAME_H

#include "graphics/drawing/drawable.h"
#include "graphics/textures/texture2d.h"


namespace arcade
{

class Game : public graphics::drawing::Drawable
{
private:
    graphics::textures::Texture2D m_texture;
    std::string m_friendlyName;
    std::string m_filename;
    
    void createFriendlyName();

public:
    Game(const std::string filename);

    graphics::textures::Texture2D& texture() { return m_texture; }
    std::string filename() const { return m_filename; }
    
};

} // namespace arcade

#endif