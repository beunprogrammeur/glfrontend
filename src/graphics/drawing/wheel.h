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

namespace graphics {
namespace drawing {
namespace wheel {

//template <typename implements_drawable> void draw(graphics::textures::Renderer& renderer,const std::vector<implements_drawable*>& drawables, int selectedIndex);



template<typename implements_drawable>
void draw(graphics::textures::Renderer &renderer, Dimensions selectedPos,
          const std::vector<implements_drawable *> &drawables, int selectedIndex)
{
    int count = drawables.size();
    if (count == 0 || (selectedPos.displacement.x == 0 && selectedPos.displacement.y == 0)) {
        return;
    }

    int screenWidth = arcade::settings::screen::width();
    int screenHeight = arcade::settings::screen::height();



    // make a copy for the current instance
    auto dim = selectedPos;

    if (dim.position.x < 0 || dim.position.x > screenWidth || dim.position.y < 0 || dim.position.y > screenHeight) {
        return;
    }


    if (dim.displacement.x < 0) { dim.displacement.x = -dim.displacement.x; }
    if (dim.displacement.y < 0) { dim.displacement.y = -dim.displacement.y; }


    int index = selectedIndex;
    while (dim.position.y > 0 &&
           dim.position.x > 0) {
        dim.position.y -= dim.displacement.y;
        dim.position.x -= dim.displacement.x;
        index--;
    }

    // make sure that the index is an item in the list and not out of bounds
    // if we only have 1 or 2 items, print those 2 alternately.
    // compensate for the upcoming modulo
    index--;

    if (index < 0) {
        index *= -1;
        index = count - index;
    }

    while (dim.position.x - (dim.size.x / 2.0f) < screenWidth &&
           dim.position.y - (dim.size.y / 2.0f) < screenHeight) {
        index = (index + 1) % count;
        auto *drawable = static_cast<Drawable *>(drawables[index]);
        if (!drawable->texture().loaded()) {
            filesystem::file::openTexture(drawable->texturePath(), drawable->texture());
        }

        renderer.draw(drawable->texture(), dim.position, dim.size);
        dim.position += dim.displacement;
    }
}

} // namespace wheel
} // namespace drawing
} // namespace graphics

#endif