#include "filesystem/file.h"
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace filesystem
{
namespace file
{

bool readText(const std::string& path, std::string& output)
{
    try
    {
        std::ifstream file(path);
        std::stringstream ss;
        ss << file.rdbuf();
        output = ss.str();
        file.close();
        return true;
    }
    catch(std::exception e)
    {
        return false;
    }
}

bool openTexture(const std::string& path, graphics::textures::Texture2D& texture)
{
    GLuint pixelFormat = GL_RGB;

    int nChannels = 0;
    GLint width;
    GLint height;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);

    if(nChannels == 4)
    {
        pixelFormat = GL_RGBA;
    }

    texture.generate(width, height, data, pixelFormat, pixelFormat);

    stbi_image_free(data);
}


} // file
} // filesystem