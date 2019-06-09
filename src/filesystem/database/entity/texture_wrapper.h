//
// Created by vincent on 19. 5. 25.
//

#ifndef GLFRONTEND_TEXTURE_WRAPPER_H
#define GLFRONTEND_TEXTURE_WRAPPER_H

#include <mutex>

#include "graphics/textures/texture2d.h"
#include "graphics/textures/image.h"

#include "filesystem/database/entity/texture_meta_info.h"

namespace filesystem {
namespace database {
namespace entity {

class TextureWrapper : public TextureMetaInfo
{
public:
    enum class State
    {
        Unknown,
        Loading,
        Ready,
        Loaded,
        Unloading,
        Unloaded
    };

private:
    State m_state;
    graphics::textures::Texture2D m_texture;
    graphics::textures::Image m_img;

public:
    TextureWrapper(int id, Type type);

    inline void state(State state)
    { m_state = state; }

    inline State state()
    { return m_state; }

    inline graphics::textures::Texture2D &texture()
    { return m_texture; }

    inline graphics::textures::Image &image()
    { return m_img; }

    // bind loaded image data to the texture on the main thread
    void finalize();
};

} // namespace entity
} // namespace database
} // namespace filesystem

#endif //GLFRONTEND_TEXTURE_WRAPPER_H
