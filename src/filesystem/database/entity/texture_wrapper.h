//
// Created by vincent on 19. 5. 25.
//

#ifndef GLFRONTEND_TEXTURE_WRAPPER_H
#define GLFRONTEND_TEXTURE_WRAPPER_H

#include <mutex>
#include "graphics/textures/texture2d.h"

namespace filesystem {
namespace database {
namespace entity {

template
class TextureWrapper
{
public:
    enum class State
    {
        Unknown,
        Loading,
        Loaded,
        Unloading,
        Unloaded
    };

private:
    graphics::textures::Texture2D m_texture;
    State m_state;
    int m_id;

public:
    explicit TextureWrapper(int id);

    inline State state() const
    { return m_state; }

    inline void state(State state)
    { m_state = state; }

    inline int id() const
    { return m_id; }

    inline graphics::textures::Texture2D &texture()
    { return m_texture; }
};

} // namespace entity
} // namespace database
} // namespace filesystem

#endif //GLFRONTEND_TEXTURE_WRAPPER_H
