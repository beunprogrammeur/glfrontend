//
// Created by vincent on 19. 5. 22.
//

#ifndef GLFRONTEND_GAME_H
#define GLFRONTEND_GAME_H

#include "filesystem/database/entity/db_entity.h"
#include <string>


namespace filesystem {
namespace database {
namespace entity {

class Game : public DBEntity
{
private:
    int m_systemId;
    std::string m_name;
    std::string m_friendlyName;
    bool m_hasImg;
    bool m_hasVid;

public:
    Game(int id, int systemId, const std::string &name, const std::string &friendly_name, bool hasImg, bool hasVid);

    inline int systemId() const
    { return m_systemId; }

    inline const std::string &name() const
    { return m_name; }

    inline const std::string &friendlyName() const
    { return m_friendlyName; }

    inline bool hasImg() const { return m_hasImg; }
    inline bool hasVid() const { return m_hasVid; }
};

} // namespace entity
} // namespace database
} // namespace filesystem


#endif //GLFRONTEND_GAME_H
