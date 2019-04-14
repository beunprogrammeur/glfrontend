#ifndef FILESYSTEM_PATH_H
#define FILESYSTEM_PATH_H

#include <string>

namespace filesystem
{
namespace path
{

std::string concat(const std::string& path, const std::string& file);
std::string escape(const std::string& input);
std::string changeExtension(const std::string& filename, const std::string& newExtension);

} // namesapce path
} // namespace filesystem

#endif