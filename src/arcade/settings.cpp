#include "settings.h"

#include "ini/ini.h"


namespace arcade {
namespace settings {
/*!
*   contains all buffered hidden variables for the settings namespace
*/
namespace {
auto ini = Ini::fromFile(SETTINGS_PATH);
}

/*!
*   calls all settings so the settings.ini file is properly initialized
*/
void init()
{
    program::title();

    screen::width();
    screen::height();
    screen::isFullScreen();

}

void save()
{
    ini.save(SETTINGS_PATH);
}

namespace debug {
bool useColor()
{ return ini.get_int("debug", "use_color", false); }
} // namespace debug


namespace program {
std::string title()
{ return ini.get("program", "title", WINDOW_TITLE); }
} // namespace program

namespace screen {
bool isFullScreen()
{ return static_cast<bool>(ini.get_int("screen", "is_fullscreen", false)); }

void isFullScreen(bool value)
{ ini.set_int("screen", "is_fullscreen", value); }

int width()
{ return ini.get_int("screen", "width", 800); }

void width(int value)
{ ini.set_int("screen", "width", value); }

int height()
{ return ini.get_int("screen", "height", 600); }

void height(int value)
{ ini.set_int("screen", "height", value); }
} // namespace screen

namespace theme {
inline std::string themeFileName()
{ return "theme.json"; }


std::string mainThemeFilePath()
{ return filesystem::path::concat(program::dataDir(), themeFileName()); }


std::string themeFilePath(std::string system)
{ return filesystem::path::concat(filesystem::path::concat(gaming::gameSystemsRootDir(), system), themeFileName()); }

} // namespace theme

namespace gaming {
std::string gameSystemsRootDir()
{ return ini.get("gaming", "game_systems_root", "./data/systems/"); }

std::string gameSystemImageName()
{ return "logo.png"; }
} // namespace gaming

namespace database {
std::string path()
{ return ini.get("database", "path", "./data/cabinet.db"); }
} // namespace database


} // namespace settings
} // namespace arcade