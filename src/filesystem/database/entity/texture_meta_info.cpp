//
// Created by vincent on 19. 6. 5.
//

#include "texture_meta_info.h"


namespace filesystem {
namespace database {
namespace entity {

TextureMetaInfo::TextureMetaInfo(int id, Type type)
    : m_type(type)
    , m_id(id)
{
}

} // namespace entity
} // namespace database
} // namespace filesystem

