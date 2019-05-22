//
// Created by vincent on 19. 5. 18.
//

#ifndef GLFRONTEND_DATABASE_H
#define GLFRONTEND_DATABASE_H
#include <vector>
#include "filesystem/database/entity/game_system.h"

namespace arcade {
    class GameSystem;
}

namespace filesystem {
namespace database {
bool init();

bool getGameSystems();

bool getGameSystems(std::vector<entity::GameSystem> &collection);
bool getGames();


} // namespace db
} // namespace filesystem

#endif //GLFRONTEND_DATABASE_H
