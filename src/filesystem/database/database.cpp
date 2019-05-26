//
// Created by vincent on 19. 5. 18.
//

#include "database.h"
#include "filesystem/file.h"

#include "debug/logger.h"
#include "arcade/settings.h"
#include "arcade/gamesystem.h"

#include <functional>
#include <sqlite3.h>
#include <vector>
#include <algorithm>

#include "filesystem/database/entity/game.h"
#include "filesystem/database/entity/game_system.h"

namespace filesystem {
namespace database {
namespace {
// private functions, members
sqlite3 *db = nullptr;
debug::Logger m_debug("database");


/*
 * Returns false if something went wrong occured. prints error to debug logger if something is up.
 *
 *
 * */
bool checkForErrors(int code, const std::string &message = "")
{
    if (code == SQLITE_OK) {
        return true;
    }
    else {
        m_debug.error(message, sqlite3_errmsg(db));
        return false;
    }
}

template<typename T>
void m_bind(sqlite3_stmt *statement, int index, T &b)
{
    m_debug.warn("bind called for non-specialized type ", typeid(b).name(), " '", b, "'");
}

template<>
void m_bind(sqlite3_stmt *statement, int index, int &b)
{
    checkForErrors(sqlite3_bind_int(statement, index, b));
}

template<>
void m_bind(sqlite3_stmt *statement, int index, bool &b)
{
    checkForErrors(sqlite3_bind_int(statement, index, b));
}

template<>
void m_bind(sqlite3_stmt *statement, int index, float &b)
{
    checkForErrors(sqlite3_bind_double(statement, index, b));
}

template<>
void m_bind(sqlite3_stmt *statement, int index, double &b)
{
    checkForErrors(sqlite3_bind_double(statement, index, b));
}

template<>
void m_bind(sqlite3_stmt *statement, int index, const char *&b)
{
    checkForErrors(sqlite3_bind_text(statement, index, b, -1, nullptr));
}

template<>
void m_bind(sqlite3_stmt *statement, int index, std::string &b)
{
    checkForErrors(sqlite3_bind_text(statement, index, b.c_str(), -1, nullptr));
}

template<typename T, typename ... args>
void m_bind(sqlite3_stmt *statement, int index, T b, args &... group)
{
    m_bind(statement, index, b);
    m_bind(statement, index + 1, group...);
}

template<typename ... args>
void bind(sqlite3_stmt *statement, args &...group)
{
    m_bind(statement, 1, group...);
}


template<typename ... args>
bool query(const std::string &query, const std::function<void(sqlite3_stmt *)> &callback, args ... bindables)
{
    sqlite3_stmt *statement = nullptr;

    int res = 0;


    if (SQLITE_OK != (res = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr))) {
        m_debug.error("failed to prepare query: ", sqlite3_errmsg(db));
        return false;
    }

    bind(statement, bindables...);


    while (SQLITE_ROW == (res = sqlite3_step(statement))) {
        if (callback != nullptr) {
            callback(statement);
        }
    }

    sqlite3_finalize(statement);

    if (res != SQLITE_OK && res != SQLITE_DONE) {
        m_debug.error("failed to execute query: ", sqlite3_errmsg(db));
        return false;
    }

    return true;
}

bool query(const std::string &query, std::function<void(sqlite3_stmt *)> callback = nullptr)
{
    sqlite3_stmt *statement = nullptr;

    int res = 0;


    if (SQLITE_OK != (res = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr))) {
        m_debug.error("failed to prepare query: \n", query, "\n\nreason: ", sqlite3_errmsg(db));
        return false;
    }

    while (SQLITE_ROW == (res = sqlite3_step(statement))) {
        if (callback != nullptr) {
            callback(statement);
        }
    }

    sqlite3_finalize(statement);

    if (res != SQLITE_OK && res != SQLITE_DONE) {
        m_debug.error("failed to execute query: ", sqlite3_errmsg(db));
        return false;
    }

    return true;
}

void createDB()
{
    query("CREATE TABLE IF NOT EXISTS game_systems( "
          "    id INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "
          "    name TEXT, "
          "    friendly_name TEXT, "
          "    rom_path TEXT, "
          "    img_path TEXT, "
          "    vid_path TEXT, "
          "    has_logo INTEGER DEFAULT 0 "
          ");");
    query("CREATE TABLE IF NOT EXISTS games ( "
          "    id INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "
          "    fk INTEGER NOT NULL, "
          "    name TEXT, "
          "    friendly_name TEXT, "
          "    has_img NOT NULL DEFAULT 0, "
          "    has_vid NOT NULL DEFAULT 0, "
          "    FOREIGN KEY(fk) REFERENCES game_systems(id) "
          ");");
}

void loadGameSystems()
{
    std::vector<arcade::GameSystem *> systems;
    arcade::GameSystem::loadSystems(systems);

    for (auto sys : systems) {
        filesystem::file::exists(sys->logoPath());

        query("INSERT INTO `game_systems`(`name`, `friendly_name`, `rom_path`, `img_path`, `vid_path`, `has_logo`)"
              "VALUES(?, ?, ?, ?, ?, ?)", nullptr,
              sys->name().c_str(),
              sys->friendlyName().c_str(),
              sys->romPath().c_str(),
              sys->imgPath().c_str(),
              sys->vidPath().c_str(),
              false);
    }
}

void loadGames(entity::GameSystem &system)
{
    std::vector<std::string> images;
    std::vector<std::string> videos;

    filesystem::file::enumerateFiles(system.imgPath(), [&](const char *file) {
        images.emplace_back(filesystem::path::changeExtension(file, ""));
    });

    filesystem::file::enumerateFiles(system.vidPath(), [&](const char *file) {
        videos.emplace_back(filesystem::path::changeExtension(file, ""));
    });

    // purge games from games for this system. (otherwise we will have duplicates)
    query("DELETE FROM games WHERE fk = ?", nullptr, system.id());

    if (query("BEGIN TRANSACTION;")) {
        filesystem::file::enumerateFiles(system.romPath(), [&](const char *file) {
            if (!query("INSERT INTO games (fk, name, friendly_name, has_img, has_vid) "
                       "VALUES(?, ?, ?, ?, ?);", nullptr,
                       system.id(),
                       file,
                       file, // TODO: make feature for friendly game names
                       std::find(images.begin(), images.end(), filesystem::path::changeExtension(file, "")) !=
                       images.end(),
                       std::find(videos.begin(), videos.end(), filesystem::path::changeExtension(file, "")) !=
                       videos.end()
            )) {
                m_debug.warn("failed to insert game '", file, "' into the database");
            }
        });
        query("COMMIT TRANSACTION;");
    }
}

void loadGames()
{
    std::vector<entity::GameSystem> systems;
    getGameSystems(systems);

    for (auto &system : systems) {
        loadGames(system);
        m_debug.print("loaded '", system.name(), "' aka '", system.friendlyName(), "'");
    }
}


void indexDB()
{
    loadGameSystems();
    loadGames();
}

bool dbIsEmpty()
{
    int count = 0;
    query("SELECT COUNT(`id`) FROM `game_systems` LIMIT 1;", [&](sqlite3_stmt *statement) {
        count = sqlite3_column_int(statement, 0);
    });

    return count == 0;
}


} // namespace [private namespace]

bool init()
{
    if (!checkForErrors(sqlite3_open(arcade::settings::database::path().c_str(), &db), "failed to open database: ")) {
        return false;
    }

    createDB();
    if (dbIsEmpty()) {
        m_debug.warn("empty database, indexing...");
        indexDB();
    }

    std::vector<entity::GameSystem> systems;
    if (!getGameSystems(systems)) {
        m_debug.warn("failed to retrieve game systems from DB");
    }

    // TODO: make resource manager
    // TODO: make background worker with mutex/semaphore that loads resources on the fly (and releases them too)
    return true;
}

bool getGameSystems(std::vector<entity::GameSystem> &collection)
{
    int count = collection.size();

    query("SELECT id, name, friendly_name, rom_path, img_path, vid_path, has_logo "
          "FROM game_systems;",
          [&](sqlite3_stmt *statement) {
              collection.emplace_back(entity::GameSystem(
                      sqlite3_column_int(statement, 0),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 3))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 4))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 5))),
                      static_cast<bool>(sqlite3_column_int(statement, 6))
              ));
          });

    return count != collection.size();
}


void getGames(entity::GameSystem &system, std::vector<entity::Game> &collection)
{
    query("SELECT id, fk, name, friendly_name, hasimg, hasvid "
          "FROM GAME "
          "WHERE fk = ?;",
          [&](sqlite3_stmt *statement) {
              collection.emplace_back(entity::Game(
                      sqlite3_column_int(statement, 0),
                      sqlite3_column_int(statement, 1),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 3))),
                      static_cast<bool>(sqlite3_column_int(statement, 4)),
                      static_cast<bool>(sqlite3_column_int(statement, 5))
              ));
          }, system.id());
}


bool getGameSystem(int id, entity::GameSystem &output)
{
    bool success = false;
    query("SELECT id, name, friendly_name, rom_path, img_path, vid_path, has_logo "
          "FROM game_systems "
          "LIMIT 1;",
          [&](sqlite3_stmt *statement) {
              output = entity::GameSystem(
                      sqlite3_column_int(statement, 0),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 3))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 4))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 5))),
                      static_cast<bool>(sqlite3_column_int(statement, 6))
              );
              success = true;
          }, id);

    return success;
}

bool getGame(int id, entity::Game &output)
{
    bool success = false;
    query("SELECT id, fk, name, friendly_name, hasimg, hasvid "
          "FROM GAME "
          "WHERE fk = ?;",
          [&](sqlite3_stmt *statement) {
              output = entity::Game(
                      sqlite3_column_int(statement, 0),
                      sqlite3_column_int(statement, 1),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2))),
                      std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 3))),
                      static_cast<bool>(sqlite3_column_int(statement, 4)),
                      static_cast<bool>(sqlite3_column_int(statement, 5))
              );
              success = true;
          }, id);

    return success;
}

} // namespace database
} // namespace filesystem
