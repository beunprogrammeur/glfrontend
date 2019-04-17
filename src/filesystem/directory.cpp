#include "filesystem/directory.h"
#include "filesystem/path.h"

#include <sstream>
#include <dirent.h>
#include <string.h>
namespace filesystem
{
namespace directory
{

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
                subDirs.push_back(path::concat(path, entry->d_name));
            }
        }
        entry = readdir(dir);
    }

    closedir(dir);
} 

} // namespace directory
} // namespace filesystem