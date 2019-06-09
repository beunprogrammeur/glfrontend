#include "filesystem/file.h"
#include "filesystem/path.h"

// std
#include <iostream>
#include <sstream>
#include <fstream>

// for directory/file enumeration
#include <dirent.h>

// processes
#include <unistd.h>
#include <stdlib.h>

// texture loading
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>


namespace filesystem {
namespace file {

bool readText(const std::string &path, std::string &output)
{
    try {
        std::ifstream file(path);
        std::stringstream ss;
        ss << file.rdbuf();
        output = ss.str();
        file.close();
        return true;
    }
    catch (std::exception e) {
        return false;
    }
}

bool openTexture(const std::string &path, graphics::textures::Texture2D &texture)
{
//    GLuint pixelFormat = GL_RGB;
//
//    int nChannels = 0;
//    GLint width;
//    GLint height;
//
//    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
//
//    if (data == nullptr) {
//        return false;
//    }
//
//    if (nChannels == 4) {
//        pixelFormat = GL_RGBA;
//    }
//
//    texture.generate(width, height, data, pixelFormat, pixelFormat);
//
//    stbi_image_free(data);
//    return true;

    graphics::textures::Image image;
    if (openImage(path, image)) {
        texture.generate(image.width, image.height, image.data, image.internalFormat, image.imgFormat);
        stbi_image_free(image.data);
        return true;
    }

    return false;
}

bool openImage(const std::string &path, graphics::textures::Image &image)
{
    GLuint pixelFormat = GL_RGB;

    int nChannels = 0;
    GLint width;
    GLint height;

    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

    if (data == nullptr) {
        return false;
    }

    if (nChannels == 4) {
        pixelFormat = GL_RGBA;
    }

    image.width = width;
    image.height = height;
    image.imgFormat = pixelFormat;
    image.internalFormat = pixelFormat;
    image.data = data;

    return true;
}

bool readJson(const std::string &path, rapidjson::Document &document)
{
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        return false;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return !document.Parse(ss.str().c_str()).HasParseError();
}


void getSubFiles(const std::string &path, std::vector<std::string> &output)
{
    DIR *dir = opendir(path.c_str());
    if (dir == nullptr) {
        return;
    }

    struct dirent *entry = readdir(dir);
    while (entry != nullptr) {
        if (entry->d_type != DT_DIR) {
            output.emplace_back((path, entry->d_name));
        }
        entry = readdir(dir);
    }

    closedir(dir);
}

bool exists(const std::string &path)
{
    if (path.empty()) {
        return false;
    }

    std::ifstream stream(path);
    return stream ? true : false;
}

bool enumerateFiles(const std::string &directory, const std::function<void(const char *name)> &callback)
{
    DIR *dir = opendir(directory.c_str());

    if (dir == nullptr) {
        return false;
    }

    struct dirent *entry = readdir(dir);

    while (entry != nullptr) {
        if (entry->d_type != DT_DIR) {
            callback(entry->d_name);
        }
        entry = readdir(dir);
    }

    closedir(dir);
}


int execute(const ::std::string &path, const std::string &args, bool escapePath, bool escapeArgs)
{
    pid_t pid = fork();
    if (pid == 0) {
        // child process

        std::stringstream ss;

        ss << (escapePath ? filesystem::path::escape(path) : path);
        ss << " ";
        ss << (escapeArgs ? filesystem::path::escape(args) : args);

        std::cout << "executing: " << ss.str() << std::endl;

        int exitcode = system(ss.str().c_str());
        exit(exitcode);
    }
    else if (pid > 0) {
        // parent process
        return pid;
    }
    else {
        // Error creating the process
        return -1;
    }
}

std::string getString(const rapidjson::Value &json, const std::string &key, const std::string &defaultValue)
{
    if (!json.HasMember(key.c_str())) {
        return defaultValue;
    }

    if (!json[key.c_str()].IsString()) {
        return defaultValue;
    }

    auto output = std::string(json[key.c_str()].GetString());
    if (output.empty()) {
        return defaultValue;
    }

    return output;
}

bool getArray(const rapidjson::Value &json, const std::string &key, std::string arr[], const int size,
              const std::string &defaultValue)
{
    if (!json.HasMember(key.c_str())) {
        return false;
    }

    if (!json[key.c_str()].IsArray()) {
        if (json[key.c_str()].IsString()) {
            for (int i = 0; i < size; i++) {
                arr[i] = std::string(json[key.c_str()].GetString());
            }
        }
        else {
            return false;
        }
    }

    auto array = json[key.c_str()].GetArray();
    int outputSize = (size > array.Size()) ? array.Size() : size;

    for (int i = 0; i < outputSize; i++) {
        arr[i] = std::string(array[i].GetString());
    }
    return true;
}


} // file
} // filesystem