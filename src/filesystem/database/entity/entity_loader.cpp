//
// Created by vincent on 19. 5. 25.
//

#include "filesystem/database/entity/entity_loader.h"
#include "game.h"
#include "game_system.h"

#include <thread>
#include <chrono>
#include "filesystem/path.h"
#include "filesystem/file.h"
#include "filesystem/database/database.h"
#include "arcade/settings.h"

namespace filesystem {
namespace database {
namespace entity {

EntityLoader::EntityLoader(int ms_sleep_bg_worker)
        : m_entities()
          , m_scheduleLoad()
          , m_msSleep(ms_sleep_bg_worker)
          , m_keep_alive(true)
          , m_bgWorker(&EntityLoader::backgroundWorker, this)
{
}

EntityLoader::~EntityLoader()
{
    m_keep_alive = false;
    m_bgWorker.join();

    for (auto &wrapper: m_entities) {
        if (wrapper.second->state() == TextureWrapper::State::Loaded) {
            // TODO: clear texture memory? (Does this work?)
            wrapper.second->texture().dispose();
        }
    }
}

bool EntityLoader::get(TextureMetaInfo &info, graphics::textures::Texture2D &texture)
{
    if (m_entities.find(info.id()) == m_entities.end()) {
        return false;
    }

    auto &wrapper = m_entities[info.id()];

    switch (wrapper->state()) {
        case TextureWrapper::State::Ready: {
            wrapper->finalize();
            texture = wrapper->texture();
            return true;
        }
        case TextureWrapper::State::Loaded:
            texture = wrapper->texture();
            return true;
        case TextureWrapper::State::Unloaded: {
            schedule(wrapper->id());

            // single threaded mode. this shoud be return false and no finalize in MT mode.
            wrapper->finalize();
            texture = wrapper->texture();
            return true;
        }
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

void EntityLoader::backgroundWorker()
{
    while (m_keep_alive) {
        if (m_scheduleLoad.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(m_msSleep));
            continue;
        }

        std::lock_guard<std::mutex> lock(m_mutex);

        int id = m_scheduleLoad.front();
        m_scheduleLoad.pop();

        auto &wrapper = m_entities[id];

        load(*wrapper);
    }
}


void EntityLoader::loadGamesFromDB(const entity::GameSystem &system)
{
    std::vector<entity::Game> games;
    database::getGames(system, games);

    for (auto &game : games) {
        m_entities[game.id()] = new TextureWrapper(game.id(), TextureWrapper::Type::Game);
    }
}

void EntityLoader::loadGameSystemsFromDB()
{
    std::vector<GameSystem> collection;
    database::getGameSystems(collection);

    for (auto &system : collection) {
        m_entities[system.id()] = new TextureWrapper(system.id(), TextureWrapper::Type::System);
    }
}

void EntityLoader::load(TextureWrapper &wrapper)
{
    if (wrapper.state() != TextureWrapper::State::Unloaded) {
        return;
    }

    wrapper.state(TextureWrapper::State::Loading);

    if (wrapper.type() == TextureMetaInfo::Type::System) {
        loadGameSystem(wrapper);
    }
    else if (wrapper.type() == TextureMetaInfo::Type::Game) {
        loadGame(wrapper);
    }
    else {
        wrapper.state(TextureWrapper::State::Unknown);
    }
}

void EntityLoader::loadGame(TextureWrapper &wrapper)
{
    Game g;
    GameSystem gs;

    if (!database::getGame(wrapper.id(), g) || database::getGameSystem(g.systemId(), gs)) {
        wrapper.state(TextureWrapper::State::Unknown);
        return;
    }
    std::string file = filesystem::path::changeExtension(g.name(), "png");
    std::string path = filesystem::path::concat(gs.imgPath(), file);
    if (filesystem::file::openImage(path, wrapper.image())) {
        wrapper.state(TextureWrapper::State::Ready);
    }
    else {
        wrapper.state(TextureWrapper::State::Unknown);
    }
}

void EntityLoader::loadGameSystem(TextureWrapper &wrapper)
{
    GameSystem gs;
    filesystem::database::getGameSystem(wrapper.id(), gs);

    std::string path = arcade::settings::gaming::gameSystemsRootDir();
    path = filesystem::path::concat(path, gs.name());
    path = filesystem::path::concat(path, arcade::settings::gaming::gameSystemImageName());
    if (filesystem::file::openImage(path, wrapper.image())) {
        wrapper.state(TextureWrapper::State::Ready);
    }
    else {
        wrapper.state(TextureWrapper::State::Unknown);
    }
}

void EntityLoader::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::queue<int> empty;
    m_scheduleLoad.swap(empty);

    for (auto &entity : m_entities) {
        entity.second->texture().dispose();
        entity.second->image().dispose();
        delete entity.second;
    }

    m_entities.clear();
}

} // namespace entity
} // namespace database
} // namespace filesystem