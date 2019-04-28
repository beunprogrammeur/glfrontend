#ifndef GRAPHICS_DRAWING_THEME_H
#define GRAPHICS_DRAWING_THEME_H

#include <vector>
#include <string>
#include <algorithm>

#include "filesystem/file.h"
#include "filesystem/path.h"

#include "graphics/resources/texture_resource.h"
#include "graphics/textures/renderer.h"

#include "debug/logger.h"

#include <rapidjson/document.h>

#include "graphics/drawing/drawable.h"
#include "graphics/drawing/dimensions.h"
#include "graphics/drawing/scenes/bgcolor_scene.h"

#include "graphics/resources/wheel_resource.h"
#include "graphics/resources/texture_resource.h"
#include "graphics/resources/bgcolor_resource.h"

namespace graphics
{
namespace drawing
{
class Theme
{
private:
    std::map<std::string, graphics::resources::Resource*> m_resources;
    std::vector<scenes::Scene*> m_scenes;
    debug::Logger m_debug;

    enum class ConversionScale
    {
        HorizontalPixels,
        VerticalPixels,
        None
    };


    template<typename drawable>
    void loadResource(const std::string &jsonRoot, const rapidjson::Value &resource, glm::vec4 &bgcolor, const std::vector<drawable*>& wheel);

    void loadScene(const rapidjson::Value& scene);
    void loadDrawableAction(scenes::Scene& scene, const rapidjson::Value& action);
    void loadBGColorAction(scenes::Scene& scene, const rapidjson::Value& action);
    static GLfloat convertUnitToNumber(const std::string& unit, GLfloat defaultValue = INT_MIN, ConversionScale scale = ConversionScale::None);
    Dimensions jsonToDimensions(const rapidjson::Value& json);
public:
     Theme();
    ~Theme();

    template <typename drawable>
    void load(const std::string &path, glm::vec4 &bgcolor, const std::vector<drawable*>& wheel);

    void draw(graphics::textures::Renderer& renderer);
    void update(GLfloat dt, glm::vec4& bgColor);
};


template <typename drawable>
void Theme::load(const std::string &path, glm::vec4 &bgcolor, const std::vector<drawable*>& wheel)
{
    rapidjson::Document d;
    if (!filesystem::file::readJson(path, d)) {
        return;
    }

    if (!d.IsObject()) {
        m_debug.error("malformed theme file. document is not an object");
        return;
    }

    std::string jsonRoot = filesystem::path::getPathWithoutFileName(path);


    if (!d.HasMember("resources")) {
        m_debug.warn("loading theme without resources");
    }
    else if (!d["resources"].IsArray()) {
        m_debug.error("malformed theme file. 'resources' is not an array");
    }
    else {
        auto resources = d["resources"].GetArray();
        for (auto &r : resources) {
            loadResource(jsonRoot, r, bgcolor, wheel);
        }
    }

    if (!d.HasMember("scenes")) {
        m_debug.warn("loading theme without scenes");
    }
    else if (!d["resources"].IsArray()) {
        m_debug.error("malformed theme file. 'scenes' is not an array");
    }
    else {
        auto scenes = d["scenes"].GetArray();
        for (auto &scene : scenes) {
            loadScene(scene);
        }
    }

    m_debug.print("loading theme complete");
}

template<typename drawable>
void Theme::loadResource(const std::string &jsonRoot, const rapidjson::Value &resource, glm::vec4 &bgcolor, const std::vector<drawable*>& wheel)
{
    std::string name = filesystem::file::getString(resource, "name");
    std::string type = filesystem::file::getString(resource, "type");
    std::string path = filesystem::file::getString(resource, "path");

    std::vector<std::string> types{
            "video",
            "sfx",
    };

    if (name.length() == 0) {
        m_debug.error("resource without name. discarding resource");
        return;
    }

    if (type != "texture" && std::find(types.begin(), types.end(), type) != types.end()) {
        m_debug.warn("type '", type, "' found but is not yet implemented. discarding resource.");
        return;
    }


    if (type != "bgcolor" && type != "wheel" && path.length() == 0) {
        m_debug.error("resource '", name, "' has no path. discarding resource");
        return;
    }


    if (type == "texture") {
        path = filesystem::path::concat(jsonRoot, path);
        auto *texResource = new graphics::resources::TextureResource(path, name);
        texResource->load();
        if (!texResource->isLoaded()) {
            delete texResource;
            m_debug.error("resource '", path, "' failed to load from disk. discarding resource");
        }
        else {
            m_resources[name] = texResource;
        }
    }
    else if (type == "bgcolor") {
        auto *bgColorResource = new graphics::resources::BGColorResource(name, bgcolor);
        m_resources[name] = bgColorResource;
    }
    else if (type == "wheel")
    {
        auto *wheelResource = new graphics::resources::WheelResource<drawable>(name, wheel);
        m_resources[name] = wheelResource;
    }
    else {
        m_debug.error("resource with unknown type '", type, "'. discarding resource");
    }
}


} // namespace drawing
} // namespace graphics

#endif