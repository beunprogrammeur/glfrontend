#include "settings.h"

#include "ini/ini.h"
#include <string>

namespace arcade
{
namespace settings
{
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



namespace program
{
    std::string title() {return ini.get("program", "title", WINDOW_TITLE); }
}

namespace screen 
{
    bool isFullScreen() { return static_cast<bool>(ini.get_int("screen", "is_fullscreen", false)); }
    void isFullScreen(bool value) {ini.set_int("screen", "is_fullscreen", value); }

    int  width() { return   ini.get_int("screen", "width", 600); }
    void width(int value) { ini.set_int("screen", "width", value); }

    int  height() { return   ini.get_int("screen", "height", 600); }
    void height(int value) { ini.set_int("screen", "height", value); }
}






} // namespace settings
} // namespace arcade