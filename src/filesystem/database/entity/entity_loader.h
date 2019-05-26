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

class EntityLoader
{
private:
    std::unordered_map<int, TextureWrapper> m_entities;
    std::queue<int> m_scheduleLoad;
    int m_msSleep;
    bool m_keep_alive;
    std::thread m_bgWorker;


    void schedule(int id);

    void backgroundWorker(bool& should_be_alive);

public:
    explicit EntityLoader(int ms_sleep_bg_worker = 10);
    ~EntityLoader();

    bool get(int id, graphics::textures::Texture2D& texture);

};

} // namespace entity
} // namespace database
} // namespace filesystem

#endif //GLFRONTEND_ENTITY_LOADER_H
