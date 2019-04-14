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

namespace graphics
{
namespace drawing
{

class Wheel
{
private:
    int m_drawCount;
    int m_itemWidth;
    int m_itemHeight;
    int m_itemMargin;
    int m_selectedPosX;
    int m_selectedPosY;

public:
    Wheel();

    template <typename implements_drawable> void draw(graphics::textures::Renderer& renderer,const std::vector<implements_drawable*>& drawables, int selectedIndex);
    
    inline int  drawCount()             const { return m_drawCount;    }
    inline void drawCount(int count)          { m_drawCount = count;   }
    inline int  itemWidth()             const { return m_itemWidth;    }
    inline void itemWidth(int width)          { m_itemWidth = width;   }
    inline int  itemHeight()            const { return m_itemHeight;   }
    inline void itemHeight(int height)        { m_itemHeight = height; }
    inline int  itemMargin()            const { return m_itemMargin;   } 
    inline void itemMargin(int margin)        { m_itemMargin = margin; }  
    inline int  selectedItemPosX()      const { return m_selectedPosX; }
    inline void selectedItemPosX(int x)       { m_selectedPosX = x;    }
    inline int  selectedItemPosY()      const { return m_selectedPosY; }
    inline void selectedItemPosY(int y)       { m_selectedPosY = y;    }
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
    
    glm::vec2 size(itemWidth(), itemHeight());
    glm::vec2 pos(selectedItemPosX(), selectedItemPosY());

    /**-- from here on the new version --**/
    int index = selectedIndex;
    while(pos.y + size.y + itemMargin() > 0)
    {
        pos.y -= size.y + itemMargin();
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

    while(pos.y < screenHeight)
    {
        index = (index + 1) % count;
        Drawable* drawable = reinterpret_cast<Drawable*>(drawables[index]);
        
        if(!drawable->texture().loaded())
        {
            filesystem::file::openTexture(drawable->texturePath(), drawable->texture());
        }

        renderer.draw(drawable->texture(), pos, size);
        pos.y += itemMargin() + size.y;
    }
}


} // namespace drawing
} // namespace graphics

#endif