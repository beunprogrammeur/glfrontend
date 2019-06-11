#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <filesystem/path.h>

namespace arcade {
namespace settings {

void init();

void save();

namespace debug {
bool useColor();
} // namespace debug

namespace program {
std::string title();

inline std::string dataDir()
{ return "./data"; }
} // namespace program

namespace screen {
bool isFullScreen();

void isFullScreen(bool value);

int width();

void width(int value);

int height();

void height(int value);
} // namespace screen

namespace gaming {
std::string gameSystemsRootDir();

std::string gameSystemImageName();
} // namespace gaming

namespace theme {
inline std::string themeFileName();
std::string mainThemeFilePath();
std::string themeFilePath(std::string system);
} // namespace theme

namespace database {
std::string path();
} // namespace database
} // namespace settings


} // namespace arcade

#endif