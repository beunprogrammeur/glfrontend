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
    std::string m_texturePath;
    
    void createFriendlyName();

public:
    Game(const std::string filename, const std::string texturePath);
    
    graphics::textures::Texture2D& texture() { return m_texture; }
    const std::string& filename() const { return m_filename; }
    const std::string& texturePath() const { return m_texturePath; } 
    void texturePath(const std::string& path) { m_texturePath; }
    
};

} // namespace arcade

#endif