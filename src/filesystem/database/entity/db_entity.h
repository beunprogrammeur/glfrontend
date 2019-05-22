//
// Created by vincent on 19. 5. 22.
//

#ifndef GLFRONTEND_DB_ENTITY_H
#define GLFRONTEND_DB_ENTITY_H

namespace filesystem {
namespace database {
namespace entity {

class DBEntity
{
private:
    int m_id;

public:
    explicit DBEntity(int id);

    inline int id() const
    { return m_id; }
};

} // namespace entity
} // namespace database
} // namespace filesystem
#endif //GLFRONTEND_DB_ENTITY_H
