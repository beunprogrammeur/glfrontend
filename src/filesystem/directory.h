#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include <string>
#include <vector>

namespace filesystem
{
namespace directory
{
    void getSubDirs(const std::string& path, std::vector<std::string>& subDirs);

} // namespace directory
} // namespace filesystem
#endif