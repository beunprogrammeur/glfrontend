#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace arcade {
namespace settings {
void init();

void save();

namespace debug {
bool useColor();
} // namespace debug

namespace program {
std::string title();
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

namespace database {
std::string path();
} // namespace database
} // namespace settings


} // namespace arcade

#endif