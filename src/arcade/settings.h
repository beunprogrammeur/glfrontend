#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

namespace arcade
{
namespace settings
{
    void init();
    void save();
    

    namespace program
    {
        std::string title();
    }

    namespace screen 
    {
        bool isFullScreen();
        void isFullScreen(bool value);
        
        int width();
        void width(int value);

        int height();
        void height(int value);
    }
}


} // namespace arcade

#endif