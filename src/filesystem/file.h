#ifndef FILESYSTEM_RESOURCE_MANAGER_H
#define FILESYSTEM_RESOURCE_MANAGER_H

// std
#include <string>
#include <vector>
#include <functional>
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// json
#include <rapidjson/document.h>

// glfrontend
#include "graphics/textures/texture2d.h"



namespace filesystem
{
namespace file
{
    bool readText(const std::string& path, std::string& output);
    bool readJson(const std::string& path, rapidjson::Document& document);
    bool openTexture(const std::string& path, graphics::textures::Texture2D& texture);
    void getSubFiles(const std::string& path, std::vector<std::string>& output);
    bool exists(const std::string& path);

    bool enumerateFiles(const std::string& directory, const std::function<void(const char* name)>& callback);

    /** Reads a string from a json file (at the given key)
    *   The requirements are that the key exists and contains a string. else the default value is used
    *   Returns: the found string / default value
    * */
    std::string getString(const rapidjson::Value& json, const std::string& key, const std::string& defaultValue = "");

    /** Reads an array from a json file at the given key
     *  The requirements are that the key exists and contains an array of strings. else the default value is used
     *  (for all the elements in the array)
     *  If the json array is shorter than the given size only "size" elements are read. it goes the same the other way
     *  Returns true if succesfully added one or more values to arr[]
    */
    bool getArray(const rapidjson::Value& json, const std::string& key, std::string arr[], const int size, const std::string& defaultValue = "");

    /*!
    *   Executes an external program given in path with the given arguments
    *   Returns: the pid of the process, -1 if it failed to create a child process
    */
    int execute(const::std::string& path, const std::string& args = "", bool escapePath = false, bool escapeArgs = false);
} // namespace file 
} // namespace filesystem

#endif