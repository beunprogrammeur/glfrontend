//
// Created by vincent on 19. 5. 18.
//

#ifndef GLFRONTEND_DATABASE_H
#define GLFRONTEND_DATABASE_H
#include <vector>

namespace filesystem {
namespace database {
namespace entity {
    class Game;
    class GameSystem;
}


bool init();

bool getGameSystems();

bool getGameSystems(std::vector<entity::GameSystem> &collection);
bool getGames();

bool getGameSystem(bool getGame(int id, entity::GameSystem& output));
bool getGame(bool getGame(int id, entity::Game& output));


} // namespace db
} // namespace filesystem

#endif //GLFRONTEND_DATABASE_H
