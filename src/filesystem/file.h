#ifndef FILESYSTEM_RESOURCE_MANAGER_H
#define FILESYSTEM_RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/textures/texture2d.h"

namespace filesystem
{
namespace file
{
    bool readText(const std::string& path, std::string& output);
    bool openTexture(const std::string& path, graphics::textures::Texture2D& texture);
    void getSubFiles(const std::string& path, std::vector<std::string>& output);

    /*!
    *   Executes an external program given in path with the given arguments
    *   Returns: the pid of the process, -1 if it failed to create a child process
    */
    int execute(const::std::string& path, const std::string& args = "", bool escapePath = false, bool escapeArgs = false);
} // namespace file 
} // namespace filesystem

#endif