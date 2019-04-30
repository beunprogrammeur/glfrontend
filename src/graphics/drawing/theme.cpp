#include "graphics/drawing/theme.h"
#include "graphics/drawing/actions/drawable_action.h"

#include "arcade/settings.h"

#include <regex>
#include <sstream>
#include <iostream>

#include "math/shuntingyard.h"

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
    for (auto pair : m_resources) {
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
        m_debug.error("found scene without name. this clashes with variables. discarding");
        return;
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
                loadDrawableAction(*myScene, action, name);
            }
        }
            break;
        case graphics::resources::Resource::Type::BGColor: {
            myScene = new scenes::BGColorScene(name, resource);
            for (auto &action : actions) {
                loadBGColorAction(*myScene, action, name);
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

void Theme::loadDrawableAction(scenes::Scene &scene, const rapidjson::Value &action, const std::string& name)
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

    if (next.empty() && id != "kill") {
        // this is considered the last action, loop to self
        // id == kill is used for a dying theme (the last thing an animating thing can do before the theme is removed
        next = id;
    }



    GLfloat durationMs = convertUnitToNumber(time, 0, ConversionScale::None);
    if(id == "kill" && durationMs > 1000.0f)
    {
        durationMs = 1000.0f;
        m_debug.warn("the kill action has a timelimit of 1000ms");
    }

    auto *myAction = new actions::DrawableAction;
    myAction->dimensions(jsonToDimensions(action, name + "." + id));
    myAction->duration(durationMs);
    myAction->resetTime();
    myAction->next(next);

    if (durationMs > 0) {
        myAction->formula(actions::Action::UpdateFormula::Linear);
    }

    scene.addAction(id, myAction);
}

Dimensions Theme::jsonToDimensions(const rapidjson::Value &json, const std::string& name)
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

    m_calculator.setHundredPercentByVariableName("screenw");
    dimensionsVariableAssignment(dimensions.size.x, size[0], name + ".size.x", Dimensions::kSizeDefault);
    m_calculator.setVariable("width", dimensions.size.x);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.size.y, size[1], name + ".size.y", Dimensions::kSizeDefault);
    m_calculator.setVariable("height", dimensions.size.y);


    m_calculator.setHundredPercentByVariableName("screenw");
    dimensionsVariableAssignment(dimensions.position.x, translate[0], name + ".translate.x", Dimensions::kPositionDefault);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.position.y, translate[1], name + ".translate.y", Dimensions::kPositionDefault);


    m_calculator.setHundredPercentByVariableName("screenw");
    dimensionsVariableAssignment(dimensions.displacement.x, displacement[0], name + ".displacement.x", Dimensions::kDisplacementDefault);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.displacement.y, displacement[1], name + ".displacement.y", Dimensions::kDisplacementDefault);

    m_calculator.setHundredPercent(360.0f);
    dimensionsVariableAssignment(dimensions.angle, targetdeg, name + ".rotation", Dimensions::kAngleDefault);

    m_calculator.setHundredPercent(1.0f);
    dimensionsVariableAssignment(dimensions.opacity, targetOpacity, name + ".opacity", Dimensions::kOpacityDefault);

    return dimensions;
}


void Theme::dimensionsVariableAssignment(float &output, std::string &expression, const std::string& variableName, float defaultValue)
{
    output = m_calculator.calculate(expression, defaultValue);
    if(output != defaultValue)
    {
        m_calculator.setVariable(variableName, output);
    }
}


void Theme::loadBGColorAction(scenes::Scene &scene, const rapidjson::Value &action, const std::string& name)
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

    auto *myAction = new actions::BGColorAction;
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