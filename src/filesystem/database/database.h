//
// Created by vincent on 19. 5. 18.
//

#ifndef GLFRONTEND_DATABASE_H
#define GLFRONTEND_DATABASE_H
#include <vector>
#include "filesystem/database/entity/texture_meta_info.h"

namespace filesystem {
namespace database {
namespace entity {
    class Game;
    class GameSystem;
}


bool init();

bool getGameSystems();

bool getGameSystems(std::vector<entity::GameSystem> &collection);
bool getGames(const entity::GameSystem &system, std::vector<entity::Game> &collection);

bool getGameSystem(int id, entity::GameSystem& system);
bool getGame(int id, entity::Game& game);

bool getGameSystemsMeta(std::vector<entity::TextureMetaInfo>& collection);
bool getGamesMeta(const entity::GameSystem& system, std::vector<entity::TextureMetaInfo>& collection);

} // namespace db
} // namespace filesystem

#endif //GLFRONTEND_DATABASE_H
