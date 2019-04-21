#include "graphics/drawing/theme.h"
#include "graphics/drawing/actions/drawable_action.h"

#include "arcade/settings.h"

#include "filesystem/file.h"
#include "filesystem/path.h"

#include <iostream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <graphics/resources/bgcolor_resource.h>
#include <graphics/drawing/actions/bgcolor_action.h>
#include "graphics/drawing/scenes/drawable_scene.h"

namespace graphics {
namespace drawing {

Theme::Theme()
        : m_resources()
          , m_debug("theme")
          , m_scenes()
{
}

Theme::~Theme()
{
    for (auto ptr : m_resources) {
        delete ptr.second;
    }

    for (auto ptr : m_scenes) {
        delete ptr;
    }
}

void Theme::load(const std::string &path, glm::vec4 &bgcolor)
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
            loadResource(jsonRoot, r, bgcolor);
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

void Theme::loadResource(const std::string &jsonRoot, const rapidjson::Value &resource, glm::vec4 &bgcolor)
{
    std::string name = filesystem::file::getString(resource, "name");
    std::string type = filesystem::file::getString(resource, "type");
    std::string path = filesystem::file::getString(resource, "path");

    std::vector<std::string> types{
            "texture",
            "video",
            "sfx",
            "wheel"
    };

    if (name.length() == 0) {
        m_debug.error("resource without name. discarding resource");
        return;
    }

    if (type.compare("texture") != 0 && std::find(types.begin(), types.end(), type) != types.end()) {
        m_debug.warn("type '", type, "' found but is not yet implemented. discarding resource.");
        return;
    }


    if (type.compare("bgcolor") != 0 && type.compare("wheel") != 0 && path.length() == 0) {
        m_debug.error("resource '", name, "' has no path. discarding resource");
        return;
    }


    if (type.compare("texture") == 0) {
        path = filesystem::path::concat(jsonRoot, path);
        graphics::resources::TextureResource *texResource = new graphics::resources::TextureResource(path, name);
        texResource->load();
        if (!texResource->isLoaded()) {
            delete texResource;
            m_debug.error("resource '", path, "' failed to load from disk. discarding resource");
        }
        else {
            m_resources[name] = texResource;
        }
    }
    else if (type.compare("bgcolor") == 0) {
        graphics::resources::BGColorResource *bgColorResource = new graphics::resources::BGColorResource(name, bgcolor);
        m_resources[name] = bgColorResource;
    }
    else {
        m_debug.error("resource with unknown type '", type, "'. discarding resource");
    }
}

void Theme::loadScene(const rapidjson::Value &scene)
{
    std::string name = filesystem::file::getString(scene, "name");
    std::string resource = filesystem::file::getString(scene, "resource");

    if (name.empty()) {
        m_debug.warn("found scene without name");
    }

    if (resource.empty()) {
        m_debug.error("ignoring scene '", name, "' without resource");
        return;
    }

    if (!scene.HasMember("actions")) {
        m_debug.warn("ignoring scene without actions");
        return;
    }

    if (!scene["actions"].IsArray()) {
        m_debug.error("ignoring. document malformed. actions is not an array");
        return;
    }

    if (m_resources.find(resource) == m_resources.end()) {
        m_debug.error("resource: ", resource, " unknown. discarding scene '", name, "'");
        return;
    }

    scenes::Scene *myScene = nullptr;

    auto actions = scene["actions"].GetArray();
    switch (m_resources[resource]->type()) {
        case graphics::resources::Resource::Type::Texture:
        case graphics::resources::Resource::Type::Video: {
            myScene = new scenes::DrawableScene(name, resource);
            for (auto &action : actions) {
                loadDrawableAction(*myScene, action);
            }
        }
            break;
        case graphics::resources::Resource::Type::BGColor: {
            myScene = new scenes::BGColorScene(name, resource);
            for (auto &action : actions) {
                loadBGColorAction(*myScene, action);
            }
        }
            break;
        default:
            m_debug.warn("resource '", resource, "' is currently of an unsupported type, discarding.");
            break;
    }

    if (myScene == nullptr) {
        m_debug.error("scene pointer is null, discarding scene '", name, "'");
        return;
    }

    m_scenes.push_back(myScene);
}

void Theme::loadDrawableAction(scenes::Scene &scene, const rapidjson::Value &action)
{
    std::string id = filesystem::file::getString(action, "id");
    if (id.empty()) {
        m_debug.error("discarding action without id");
        return;
    }

    std::string time = filesystem::file::getString(action, "time");
    std::string next = filesystem::file::getString(action, "next");
    std::string targetdeg = filesystem::file::getString(action, "rotation");
    std::string targetOpacity = filesystem::file::getString(action, "rotation");
    std::string translate[2] = {""};
    std::string size[2] = {""};


    if (time.empty()) {
        time = "0";
    }

    if (next.empty()) {
        // this is considered the last action, loop to self
        next = id;
    }

    if (targetdeg.empty()) {
        targetdeg = "0";
    }

    if (targetOpacity.empty()) {
        targetOpacity = "1.0";
    }

    filesystem::file::getArray(action, "translate", translate, sizeof(translate), "-2147483648");
    filesystem::file::getArray(action, "size", size, sizeof(size), "-2147483648");
    Dimensions dimensions;


    dimensions.position.x = convertUnitToNumber(translate[0], Dimensions::kPositionDefault,
                                                ConversionScale::HorizontalPixels);
    dimensions.position.y = convertUnitToNumber(translate[1], Dimensions::kPositionDefault,
                                                ConversionScale::VerticalPixels);
    dimensions.size.x = convertUnitToNumber(size[0], Dimensions::kSizeDefault, ConversionScale::HorizontalPixels);
    dimensions.size.y = convertUnitToNumber(size[1], Dimensions::kSizeDefault, ConversionScale::VerticalPixels);
    dimensions.angle = convertUnitToNumber(targetdeg, Dimensions::kAngleDefault, ConversionScale::None);
    dimensions.opacity = convertUnitToNumber(targetOpacity, Dimensions::kOpacityDefault, ConversionScale::None);
    int durationMs = convertUnitToNumber(time, 0, ConversionScale::None);


    actions::DrawableAction *myAction = new actions::DrawableAction;
    myAction->dimensions(dimensions);
    myAction->duration(durationMs);
    myAction->resetTime();
    myAction->next(next);

    if (durationMs > 0) {
        myAction->formula(actions::Action::UpdateFormula::Linear);
    }

    scene.addAction(id, myAction);
}

void Theme::loadBGColorAction(scenes::Scene &scene, const rapidjson::Value &action)
{
    std::string id = filesystem::file::getString(action, "id");
    if (id.empty()) {
        m_debug.error("discarding action without id");
        return;
    }

    std::string time = filesystem::file::getString(action, "time");
    std::string next = filesystem::file::getString(action, "next");


    if (time.empty()) {
        time = "0";
    }

    if (next.empty()) {
        // this is considered the last action, loop to self
        next = id;
    }
    std::string color[3]{};
    filesystem::file::getArray(action, "color", color, sizeof(color), "-1.0");

    GLfloat r = convertUnitToNumber(color[0], 0.0f, ConversionScale::None);
    GLfloat g = convertUnitToNumber(color[1], 0.0f, ConversionScale::None);
    GLfloat b = convertUnitToNumber(color[2], 0.0f, ConversionScale::None);
    int durationMs = convertUnitToNumber(time, 0, ConversionScale::None);

    actions::BGColorAction *myAction = new actions::BGColorAction;
    myAction->red(r);
    myAction->green(g);
    myAction->blue(b);
    myAction->duration(durationMs);
    myAction->resetTime();
    myAction->next(next);

    if (durationMs > 0) {
        myAction->formula(actions::Action::UpdateFormula::Linear);
    }

    scene.addAction(id, myAction);
}

/* Converts unit numbers in the json file to actual pixels
*  example 100% (Horizontal scale) == screen width
*  example 40px == 40 pixels
*  example 4000 == 4000 pixels
*/
GLfloat Theme::convertUnitToNumber(const std::string &unit, int defaultValue, ConversionScale scale)
{
    std::regex re("^([0-9.]+)(px|%)?");
    std::smatch match;
    if (std::regex_search(unit, match, re)) {
        std::string number = match[1];
        std::string unit = match[2];
        GLfloat num = 0;
        std::istringstream ss(number);
        ss >> num;
        if (ss.good()) {
            return defaultValue;
        }

        if (unit.empty() || unit.compare("px") == 0) {
            return num;
        }
        else {
            // percentual unit scale %
            int resolutionAxisSize = arcade::settings::screen::width();
            if (scale == ConversionScale::VerticalPixels) {
                resolutionAxisSize = arcade::settings::screen::height();
            }

            return resolutionAxisSize * num / 100; // divide by 100 because of the percentual scale
        }
    }
    else {
        return defaultValue;
    }
}

void Theme::draw(graphics::textures::Renderer &renderer)
{
    for (auto &scene: m_scenes) {
        if (auto ptr = dynamic_cast<scenes::DrawableScene *>(scene)) {
            ptr->draw(renderer, *static_cast<resources::DrawableResource *>(m_resources[ptr->resourceId()]));
        }
        else if (auto ptr = dynamic_cast<scenes::BGColorScene *>(scene)) {
            ptr->draw(renderer, *static_cast<resources::BGColorResource *>(m_resources[ptr->resourceId()]));
        }
        else {
            // unsupported type of scene
        }
    }
}

void Theme::update(GLfloat dt, glm::vec4 &bgColor, Dimensions &wheelDimensions)
{
    for (auto scene : m_scenes) {
        scene->update(dt);
    }
}


} // namespace drawing
} // namespace graphics