//
// Created by vincent on 19. 5. 25.
//

#include "texture_wrapper.h"

namespace filesystem {
namespace database {
namespace entity {

TextureWrapper::TextureWrapper(int id)
    : m_texture()
    , m_state(State::Unloaded)
    , m_id(id)
{
}

} // namespace entity
} // namespace database
} // namespace filesystem
