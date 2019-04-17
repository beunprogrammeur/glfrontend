#include "graphics/drawing/theme.h"
#include "graphics/drawing/action.h"

#include "arcade/settings.h"

#include "filesystem/file.h"
#include "filesystem/path.h"

#include <iostream>
#include <sstream>
#include <regex>



namespace graphics
{
namespace drawing
{

Theme::Theme()
    : m_textures()
    , m_debug("theme")
    , m_scenes()
{
}

Theme::~Theme()
{
    for(auto ptr : m_textures)
    {
        delete ptr.second;
    }
}

void Theme::load(const std::string& path)
{
    rapidjson::Document d;
    if(!filesystem::file::readJson(path, d))
    {
        return;
    }  

    if(!d.IsObject())
    {
        m_debug.error("malformed theme file. document is not an object");
        return;
    }

    std::string jsonRoot = filesystem::path::getPathWithoutFileName(path);

    
    if(!d.HasMember("resources"))
    {
        m_debug.warn("loading theme without resources");
    }
    else if(!d["resources"].IsArray())
    {
        m_debug.error("malformed theme file. 'resources' is not an array");
    }
    else
    {
        auto resources = d["resources"].GetArray();
        for(auto& r : resources)
        {
            loadResource(jsonRoot, r);
        }
    }
       
    if(!d.HasMember("scenes"))
    {
        m_debug.warn("loading theme without scenes");
    }
    else if(!d["resources"].IsArray())
    {
        m_debug.error("malformed theme file. 'scenes' is not an array");
    }
    else
    {
        auto scenes = d["scenes"].GetArray();
        for(auto& scene : scenes)
        {
            loadScene(scene);
        }
    }

    m_debug.print("loading theme complete");
}

void Theme::loadResource(const std::string & jsonRoot, const rapidjson::Value& resource)
{
    std::string name = filesystem::file::getString(resource, "name");
    std::string type = filesystem::file::getString(resource, "type");
    std::string path = filesystem::file::getString(resource, "path");
    
    if(name.length() == 0)
    {
        m_debug.error("resource without name. discarding resource");
        return;
    }

    if(path.length() == 0)
    {
        m_debug.error("resource '", name, "' has no path. discarding resource");
        return;
    }

    if(type.compare("texture") == 0)
    {  
        path = filesystem::path::concat(jsonRoot, path);
        graphics::resources::TextureResource* texResource = new graphics::resources::TextureResource(path, name);
        texResource->load();
        if(!texResource->isLoaded())
        {
            delete texResource;
            m_debug.error("resource '", path, "' failed to load from disk. discarding resource");
        }
        else
        {
            m_textures[name] = texResource;
        }
    }
    else
    {
        m_debug.error("resource with unknown type '", type, "'. discarding resource");
    }
}

void Theme::loadScene(const rapidjson::Value& scene)
{
    std::string name     = filesystem::file::getString(scene, "name");
    std::string resource = filesystem::file::getString(scene, "resource");

    if(name.empty())
    {
        m_debug.warn("found scene without name");
    }

    if(resource.empty())
    {
        m_debug.error("ignoring scene without resource");
        return;
    }

    if(!scene.HasMember("actions"))
    {
        m_debug.warn("ignoring scene without actions");
        return;
    }

    if(!scene["actions"].IsArray())
    {
        m_debug.error("ignoring. document malformed. actions is not an array");
        return;
    }

    if(m_textures.find(resource) == m_textures.end())
    {
        m_debug.error("resource: ", resource, " unknown. discarding scene");
        return;
    }

    Scene myScene(name, resource);
    auto actions = scene["actions"].GetArray();
    for(auto& action : actions)
    {
        loadAction(myScene, action);
    }

    m_scenes.push_back(myScene);
}

void Theme::loadAction(Scene& scene, const rapidjson::Value& action)
{
    Action myAction;
    std::string id =filesystem::file::getString(action, "id");
    if(id.empty())
    {
        m_debug.error("discarding action without id");
        return;
    }

    std::string time = filesystem::file::getString(action, "time");
    std::string next = filesystem::file::getString(action, "next");
    std::string targetdeg     = filesystem::file::getString(action, "rotation");
    std::string targetOpacity = filesystem::file::getString(action, "rotation");
    std::string translate[2] = {""}; // TODO fill arrays with getArray()
    std::string size[2]      = {""};
    

    if(time.empty())
    {
        time = "0";
    }

    if(next.empty())
    {
        // this is considered the last action, loop to self
        next = id;
    }

    if(targetdeg.empty())
    {
        targetdeg = "0";
    }

    if(targetOpacity.empty())
    {
        targetOpacity = "1.0";
    }

    // TODO: parse targetdeg, opacity and wait

    filesystem::file::getArray(action, "translate", translate, sizeof(translate), "-2147483648");
    filesystem::file::getArray(action, "size", size, sizeof(size), "-2147483648"); 

    myAction.targetX(convertUnitToNumber(translate[0], INT_MIN, ConversionScale::HorizontalPixels));
    myAction.targetY(convertUnitToNumber(translate[1], INT_MIN, ConversionScale::VerticalPixels));
    myAction.targetWidth(convertUnitToNumber(size[0],  INT_MIN, ConversionScale::HorizontalPixels));
    myAction.targetHeight(convertUnitToNumber(size[1], INT_MIN, ConversionScale::VerticalPixels));
    myAction.targetDegrees(convertUnitToNumber(targetdeg, 0, ConversionScale::None));
    myAction.targetOpacity(convertUnitToNumber(targetOpacity, 1, ConversionScale::None));
    myAction.actionDuration(convertUnitToNumber(time, 0, ConversionScale::None));
    myAction.resetTime();
    myAction.next(next);

    scene.addAction(id, myAction);
}

/* Converts unit numbers in the json file to actual pixels
*  example 100% (Horizontal scale) == screen width
*  example 40px == 40 pixels
*  example 4000 == 4000 pixels
*/
int Theme::convertUnitToNumber(const std::string& unit, int defaultValue, ConversionScale scale)
{
    std::regex re("^([0-9.]+)(px|%)?");
    std::smatch match;
    if(std::regex_search(unit, match, re))
    {
        std::string number = match[1];
        std::string unit   = match[2];
        int num = 0;
        std::istringstream ss(number);
        ss >> num;
        if(ss.good())
        {
            return defaultValue;            
        }

        if(unit.empty() || unit.compare("px") == 0)
        {
            return num;
        }
        else
        {
            // percentual unit scale %
            int resolutionAxisSize = arcade::settings::screen::width();
            if(scale == ConversionScale::VerticalPixels)
            {
                resolutionAxisSize = arcade::settings::screen::height();
            }

            return resolutionAxisSize * num / 100; // divide by 100 because of the percentual scale
        }
    }
    else
    {
        return defaultValue;
    }
}

void Theme::draw(graphics::textures::Renderer& renderer)
{
    for(auto& scene: m_scenes)
    {
        scene.draw(renderer, *m_textures[scene.resourceId()]);
    }
}

void Theme::update(GLfloat dt)
{
    for(auto& scene: m_scenes)
    {
        scene.update(dt);
    }
}


} // namespace drawing
} // namespace graphics