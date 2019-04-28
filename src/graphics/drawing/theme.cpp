#include "graphics/drawing/theme.h"
#include "graphics/drawing/actions/drawable_action.h"

#include "arcade/settings.h"

#include <regex>
#include <sstream>
#include <iostream>

#include "graphics/drawing/actions/bgcolor_action.h"
#include "graphics/drawing/scenes/drawable_scene.h"
#include "wheel.h"

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
    for(auto pair : m_resources) {
        delete pair.second;
    }

    for (auto ptr : m_scenes) {
        delete ptr;
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
        case graphics::resources::Resource::Type::Video:
        case graphics::resources::Resource::Type::Wheel: {
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


    if (time.empty()) {
        time = "0";
    }

    if (next.empty()) {
        // this is considered the last action, loop to self
        next = id;
    }


    GLfloat durationMs = convertUnitToNumber(time, 0, ConversionScale::None);


    auto* myAction = new actions::DrawableAction;
    myAction->dimensions(jsonToDimensions(action));
    myAction->duration(durationMs);
    myAction->resetTime();
    myAction->next(next);

    if (durationMs > 0) {
        myAction->formula(actions::Action::UpdateFormula::Linear);
    }

    scene.addAction(id, myAction);
}

Dimensions Theme::jsonToDimensions(const rapidjson::Value& json)
{

    std::string targetdeg = filesystem::file::getString(json, "rotation");
    std::string targetOpacity = filesystem::file::getString(json, "opacity");
    std::string displacement[2] = {""};
    std::string translate[2] = {""};
    std::string size[2] = {""};


    if (targetdeg.empty()) {
        targetdeg = "0.0";
    }

    if (targetOpacity.empty()) {
        targetOpacity = "1.0";
    }


    filesystem::file::getArray(json, "translate", translate, sizeof(translate),
            std::to_string(Dimensions::kPositionDefault));
    filesystem::file::getArray(json, "size", size, sizeof(size),
            std::to_string(Dimensions::kPositionDefault));
    filesystem::file::getArray(json, "displacement", displacement, sizeof(displacement),
            std::to_string(Dimensions::kDisplacementDefault));

    Dimensions dimensions;

    dimensions.position.x = convertUnitToNumber(translate[0],
            Dimensions::kPositionDefault, ConversionScale::HorizontalPixels);
    dimensions.position.y = convertUnitToNumber(translate[1],
            Dimensions::kPositionDefault, ConversionScale::VerticalPixels);
    dimensions.size.x = convertUnitToNumber(size[0],
            Dimensions::kSizeDefault, ConversionScale::HorizontalPixels);
    dimensions.size.y = convertUnitToNumber(size[1],
            Dimensions::kSizeDefault, ConversionScale::VerticalPixels);
    dimensions.angle = convertUnitToNumber(targetdeg,
            Dimensions::kAngleDefault, ConversionScale::None);
    dimensions.opacity = convertUnitToNumber(targetOpacity,
            Dimensions::kOpacityDefault, ConversionScale::None);
    dimensions.displacement.x = convertUnitToNumber(displacement[0],
            Dimensions::kDisplacementDefault, ConversionScale::HorizontalPixels);
    dimensions.displacement.y = convertUnitToNumber(displacement[1],
            Dimensions::kDisplacementDefault, ConversionScale::VerticalPixels);

    return dimensions;
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
GLfloat Theme::convertUnitToNumber(const std::string &unit, GLfloat defaultValue, ConversionScale scale)
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
            ptr->draw(renderer, m_resources[ptr->resourceId()]);
        }
        else if (auto ptr = dynamic_cast<scenes::BGColorScene *>(scene)) {
            ptr->draw(renderer, *dynamic_cast<resources::BGColorResource *>(m_resources[ptr->resourceId()]));
        }
        else {
            // unsupported type of scene
        }
    }
}

void Theme::update(GLfloat dt, glm::vec4 &bgColor)
{
    for (auto scene : m_scenes) {
        scene->update(dt);
    }
}


} // namespace drawing
} // namespace graphics