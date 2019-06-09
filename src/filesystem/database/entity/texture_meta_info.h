//
// Created by vincent on 19. 6. 5.
//

#ifndef GLFRONTEND_TEXTURE_META_INFO_H
#define GLFRONTEND_TEXTURE_META_INFO_H


namespace filesystem {
namespace database {
namespace entity {

class TextureMetaInfo
{
public:
    enum class Type
    {
        System,
        Game
    };

    TextureMetaInfo(int id, Type type);

    inline int id() const
    { return m_id; }

    Type type()
    { return m_type; }

private:
    Type m_type;
    int m_id;
};

} // namespace entity
} // namespace database
} // namespace filesystem


#endif //GLFRONTEND_TEXTURE_META_INFO_H
