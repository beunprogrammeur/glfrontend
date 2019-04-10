#ifndef FILESYSTEM_RESOURCE_MANAGER_H
#define FILESYSTEM_RESOURCE_MANAGER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "graphics/textures/texture2d.h"

namespace filesystem
{
namespace file
{
    bool readText(const std::string& path, std::string& output);
    bool openTexture(const std::string& path, graphics::textures::Texture2D& texture);

} // namespace file 
} // namespace filesystem

#endif