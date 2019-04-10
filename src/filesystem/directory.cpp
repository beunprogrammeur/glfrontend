#include "filesystem/directory.h"
#include <sstream>
#include <dirent.h>
#include <string.h>

namespace filesystem
{
namespace directory
{

std::string concat(const std::string& path, const std::string& file)
{
    if(path.length() == 0)
    {
        return file;
    }

    int lastCharIndex = path.length() -1;
    if(path[lastCharIndex] == '/' ||
       path[lastCharIndex] == '\\')
    {
        lastCharIndex--;
    }

    std::stringstream ss;
    ss << path.substr(0, lastCharIndex+1) << "/" << file;
    return ss.str();
}


    void getSubDirs(const std::string& path, std::vector<std::string>& subDirs)
    {
        DIR* dir = opendir(path.c_str());
        if(dir == nullptr)
        {
            return;
        }

        struct dirent* entry = readdir(dir);
        while(entry != nullptr)
        {
            if(entry->d_type == DT_DIR)
            {
                if(strcmp(entry->d_name, ".")  != 0 &&
                   strcmp(entry->d_name, "..") != 0)
                {
                    subDirs.push_back(concat(path, entry->d_name));
                }
            }
            entry = readdir(dir);
        }
    } 

} // namespace directory
} // namespace filesystem