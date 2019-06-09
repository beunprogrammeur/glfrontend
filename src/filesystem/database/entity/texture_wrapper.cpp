//
// Created by vincent on 19. 5. 25.
//

#include <stb_image.h>
#include "texture_wrapper.h"

namespace filesystem {
namespace database {
namespace entity {

TextureWrapper::TextureWrapper(int id, Type type)
        : TextureMetaInfo(id, type)
          , m_state(State::Unloaded)
          , m_texture()
{
}

void TextureWrapper::finalize()
{
    if (m_img.data != nullptr) {
        m_texture.generate(m_img);
        m_state = State::Loaded;
    }
}

} // namespace entity
} // namespace database
} // namespace filesystem
