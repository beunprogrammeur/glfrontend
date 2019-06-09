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
std::string getPathWithoutFileName(const std::string& path);
std::string getFileName(const std::string& path);
} // namesapce path
} // namespace filesystem

#endif