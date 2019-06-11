#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include <string>
#include <functional>

namespace filesystem
{
namespace directory
{
    void getSubDirs(const std::string& path, std::function<void(const char*)> callback);

} // namespace directory
} // namespace filesystem
#endif