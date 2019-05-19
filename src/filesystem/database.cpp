//
// Created by vincent on 19. 5. 18.
//

#include "filesystem/database.h"
#include "debug/logger.h"
#include "arcade/settings.h"
#include <sqlite3.h>

namespace filesystem {
namespace database {
namespace {
// private functions, members
sqlite3 *db = nullptr;
debug::Logger m_debug("database");


bool checkForErrors(int code, const std::string &message)
{
    if (code == 0) {
        return true;
    }
    else {
        m_debug.error(message, sqlite3_errmsg(db));
        return false;
    }
}
}

bool init()
{
    if (checkForErrors(sqlite3_open(arcade::settings::database::path().c_str(), &db), "failed to open database: ")) {
        return false;
    }

    // TODO: create table if not exists queries
    // TODO: load game systems

    // TODO: make resource manager
    // TODO: make background worker with mutex/semaphore that loads resources on the fly (and releases them too)
    return true;
}


} // db
} // namespace filesystem