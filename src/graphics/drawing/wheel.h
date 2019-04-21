#ifndef GRAPHICS_DRAWING_WHEEL_H
#define GRAPHICS_DRAWING_WHEEL_H
#include <vector>
#include <cmath>

#include "graphics/textures/renderer.h"
#include "graphics/drawing/drawable.h"

#include "arcade/game.h"
#include "arcade/gamesystem.h"
#include "arcade/settings.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "filesystem/file.h"
#include "graphics/drawing/dimensions.h"

namespace graphics
{
namespace drawing
{

class Wheel
{
private:
    int m_drawCount;
    graphics::drawing::Dimensions m_selectedItemDimensions;

public:
    Wheel();

    template <typename implements_drawable> void draw(graphics::textures::Renderer& renderer,const std::vector<implements_drawable*>& drawables, int selectedIndex);

    inline int  drawCount()    const { return m_drawCount;    }
    inline void drawCount(int count) { m_drawCount = count;   }
    inline Dimensions& dimensions()  { return m_selectedItemDimensions; }
};


template <typename implements_drawable>
void Wheel::draw(graphics::textures::Renderer& renderer, const std::vector<implements_drawable*>& drawables, int selectedIndex)
{
    int count = drawables.size();
    if(count == 0)
    {
        return;
    }

    int screenWidth  = arcade::settings::screen::width();
    int screenHeight = arcade::settings::screen::height();

    // make a copy for the current instance
    auto dim = m_selectedItemDimensions;

    /**-- from here on the new version --**/
    int index = selectedIndex;
    while(dim.position.y + dim.size.y + dim.margin.y > 0)
    {
        dim.position.y -= dim.size.y + dim.margin.y;
        index = (index-1) % count;
    }

    // make sure that the index is an item in the list and not out of bounds
    // if we only have 1 or 2 items, print those 2 alternately.
    // compensate for the upcoming modulo
    index--;

    if(index < 0)
    {
        index *= -1;
        index = count - index;
    }

    while(dim.position.y < screenHeight)
    {
        index = (index + 1) % count;
        Drawable* drawable = reinterpret_cast<Drawable*>(drawables[index]);
        
        if(!drawable->texture().loaded())
        {
            filesystem::file::openTexture(drawable->texturePath(), drawable->texture());
        }

        renderer.draw(drawable->texture(), dim.position, dim.size);
        dim.position.y += dim.margin.y + dim.size.y;
    }
}


} // namespace drawing
} // namespace graphics

#endif