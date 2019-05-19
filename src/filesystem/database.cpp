//
// Created by vincent on 19. 5. 18.
//

#include "filesystem/database.h"
#include "debug/logger.h"
#include "arcade/settings.h"
#include <sqlite3.h>
#include <vector>

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
bool checkForErrors(int code, const std::string &message)
{
    if (code == SQLITE_OK) {
        return true;
    }
    else {
        m_debug.error(message, sqlite3_errmsg(db));
        return false;
    }
}

void createDB()
{
    std::vector<std::string> queries;
    queries.emplace_back("CREATE TABLE IF NOT EXISTS `game_systems`( " \
            "    `id` INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "\
            "    `name` TEXT, "\
            "    `rom_path` TEXT, "\
            "    `img_path` TEXT, "\
            "    `vid_path` TEXT, "\
            "    `has_logo` INTEGER DEFAULT 0 "\
            ")");


    queries.emplace_back("CREATE TABLE IF NOT EXISTS `games` ( "\
            "    `id` INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "\
            "    `fk` INTEGER NOT NULL, "\
            "    `name` TEXT, " \
            "    `friendly_name` TEXT, "\
            "    FOREIGN KEY(`fk`) REFERENCES `game_systems`(`id`) "
                         ")");

    queries.emplace_back("CREATE TABLE IF NOT EXISTS `images` ( "\
            "    `id` INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "\
            "    `fk` INTEGER, "\
            "    FOREIGN KEY(`fk`) REFERENCES `games`(`id`) "\
            ")");

    queries.emplace_back("CREATE TABLE IF NOT EXISTS `videos` ( "\
            "    `id` INTEGER NOT NULL DEFAULT 1 PRIMARY KEY AUTOINCREMENT UNIQUE, "\
            "    `fk` INTEGER, "\
            "    FOREIGN KEY(`fk`) REFERENCES `games`(`id`) "\
            ")");

    for (auto &query : queries) {
        if(sqlite3_exec(db, query.c_str(), nullptr, 0, nullptr) != SQLITE_OK) {
            m_debug.error("failed to create database tables: ", sqlite3_errmsg(db));
        }
    }
}

}

bool init()
{
    if (!checkForErrors(sqlite3_open(arcade::settings::database::path().c_str(), &db), "failed to open database: ")) {
        return false;
    }

    createDB();
    // TODO: load game systems

    // TODO: make resource manager
    // TODO: make background worker with mutex/semaphore that loads resources on the fly (and releases them too)
    return true;
}


} // db
} // namespace filesystem