//
// Created by vincent on 19. 5. 25.
//

#include "filesystem/database/entity/entity_loader.h"
#include "game.h"
#include "game_system.h"

#include<thread>
#include<chrono>
#include <filesystem/path.h>
#include <filesystem/file.h>
#include <filesystem/database/database.h>

namespace filesystem {
namespace database {
namespace entity {

EntityLoader::EntityLoader(int ms_sleep_bg_worker)
        : m_entities()
          , m_scheduleLoad()
          , m_msSleep(ms_sleep_bg_worker)
          , m_keep_alive(true)
          , m_bgWorker(m_bgWorker, m_keep_alive)
{
}

EntityLoader::~EntityLoader()
{
    m_keep_alive = false;
    m_bgWorker.join();

    for (auto &wrapper: m_entities) {
        if (wrapper.second.state() == TextureWrapper::State::Loaded)) {
            // TODO: clear texture memory
        }
    }
}

bool EntityLoader::get(int id, graphics::textures::Texture2D &texture)
{
    if (m_entities.find(id) == m_entities.end()) {
        return false;
    }

    auto &wrapper = m_entities[id];

    switch (wrapper.state()) {
        case TextureWrapper::State::Loaded:
            texture = wrapper.texture();
            return true;
        case TextureWrapper::State::Unloaded:
            schedule(wrapper.id());
            return false;
        case TextureWrapper::State::Unloading:  // fall through
        case TextureWrapper::State::Unknown:    // fall through
        case TextureWrapper::State::Loading:    // fall through
        default:
            return false;
    }
}

void EntityLoader::schedule(int id)
{
    m_scheduleLoad.push(id);
}

void EntityLoader::backgroundWorker(bool &should_be_alive)
{
    while (should_be_alive) {
        if (m_scheduleLoad.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_msSleep));
            continue;
        }

        int id = m_scheduleLoad.front();
        m_scheduleLoad.pop();

        auto &wrapper = m_entities[id];

        if (wrapper.state() != TextureWrapper::State::Unloaded) {
            continue;
        }

        wrapper.state(TextureWrapper::State::Loading);
        Game g;
        GameSystem gs;

        if (filesystem::database::getGame(id, g) &&
            filesystem::database::getGameSystem(g.systemId(), gs)) {
            std::string imgName = filesystem::path::changeExtension(g.name(), ".png");
            std::string path = filesystem::path::concat(gs.imgPath(), imgName);
            filesystem::file::openTexture(path, wrapper.texture());
        }

        if (wrapper.texture().loaded()) {
            wrapper.state(TextureWrapper::State::Loaded);
        }
        else {
            wrapper.state(TextureWrapper::State::Unknown);
        }
    }
}

} // namespace entity
} // namespace database
} // namespace filesystem