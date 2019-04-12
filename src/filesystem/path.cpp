#include "filesystem/path.h"
#include <sstream>
#include <algorithm>

namespace filesystem
{
namespace path
{

std::string concat(const std::string& path, const std::string& file)
{
    if(path.length() == 0)
    {
        return file;
    }

    int lastCharIndex = path.length() -1;
    if(path[lastCharIndex] == '/' ||
       path[lastCharIndex] == '\\')
    {
        lastCharIndex--;
    }

    std::stringstream ss;
    ss << path.substr(0, lastCharIndex+1) << "/" << file;
    return ss.str();
}


std::string escape(const std::string& input)
{
    char escape[] = {'\\', ' ', '\'', '(', ')'};
    std::stringstream ss;

    for(char c : input)
    {
        if(std::find(std::begin(escape), std::end(escape), c) != std::end(escape))
        {
            ss << '\\';
        }
        ss << c;
    }

    return ss.str();
}

} // namespace path
} // namespace filesystem