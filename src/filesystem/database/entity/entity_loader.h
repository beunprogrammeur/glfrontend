//
// Created by vincent on 19. 5. 25.
//

#ifndef GLFRONTEND_ENTITY_LOADER_H
#define GLFRONTEND_ENTITY_LOADER_H

#include <unordered_map>
#include <queue>
#include <thread>

#include "filesystem/database/entity/texture_wrapper.h"

namespace filesystem {
namespace database {
namespace entity {

class GameSystem; // forward declare

class EntityLoader
{
private:
    std::unordered_map<int, TextureWrapper*> m_entities;
    std::queue<int> m_scheduleLoad;
    int m_msSleep;
    bool m_keep_alive;
    std::thread m_bgWorker;
    std::mutex m_mutex;

    void schedule(int id);

    void backgroundWorker();
    static void load(TextureWrapper& wrapper);
    static void loadGame(TextureWrapper& wrapper);
    static void loadGameSystem(TextureWrapper& wrapper);

public:
    explicit EntityLoader(int ms_sleep_bg_worker = 10);
    ~EntityLoader();

    bool get(TextureMetaInfo& info, graphics::textures::Texture2D& texture);

    void loadGamesFromDB(const GameSystem& system);

    void loadGameSystemsFromDB();

    void clear();
};

} // namespace entity
} // namespace database
} // namespace filesystem

#endif //GLFRONTEND_ENTITY_LOADER_H
