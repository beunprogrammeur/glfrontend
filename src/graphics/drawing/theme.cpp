#include "graphics/drawing/theme.h"

#include "arcade/settings.h"

#include <regex>
#include <sstream>
#include <iostream>
#include "filesystem/database/database.h"
#include "filesystem/database/entity/game.h"
#include "filesystem/file.h"
#include "filesystem/path.h"

#include "math/shuntingyard.h"

#include "graphics/drawing/actions/bgcolor_action.h"
#include "graphics/drawing/actions/drawable_action.h"
#include "graphics/drawing/actions/wheel_action.h"
#include "graphics/drawing/scenes/drawable_scene.h"
#include "graphics/drawing/scenes/wheel_scene.h"
#include "graphics/resources/wheel_resource.h"
#include "graphics/resources/texture_resource.h"
#include "graphics/resources/bgcolor_resource.h"

#include "filesystem/database/entity/game_system.h"


#include "arcade/settings.h"


namespace graphics {
namespace drawing {

Theme::Theme()
        : m_resources()
          , m_scenes()
          , m_debug("theme")
          , m_calculator()
          , m_wheelDrawables()
          , m_wheelImageLoader()
          , m_wheelIndex(0)
          , m_indexDifferential(0)
          , m_indexChanged(false)
          , m_dying(false)
          , m_killCallback(nullptr)
{
}

Theme::~Theme()
{
    dispose();
}


void Theme::dispose()
{
    for (auto pair : m_resources) {
        delete pair.second;
    }

    for (auto ptr : m_scenes) {
        delete ptr;
    }

    m_resources.clear();
    m_scenes.clear();
    m_calculator.reset();
    m_wheelDrawables.clear();
    m_wheelImageLoader.clear();
    m_wheelIndex = 0;
    m_indexDifferential = 0;
    m_indexChanged = false;
    m_dying = false;
    m_killCallback = nullptr;
}


void Theme::kill(std::function<void()> callback)
{
    if (m_dying) {
        return;
    }

    m_dying = true;
    m_killCallback = callback;

    bool hasKillable = false;
    for (auto ptr : m_scenes) {
        if (ptr->kill()) {
            hasKillable = true;
        }
    }

    // if there is no killable scene instant kill theme
    // else wait for  scenes to die
    if (!hasKillable) {
        callback();
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
        case graphics::resources::Resource::Type::Video: {
            myScene = new scenes::DrawableScene(name, resource);
            for (auto &action : actions) {
                auto *drawableAction = loadDrawableAction(action, name);
                if (drawableAction != nullptr) {
                    myScene->addAction(drawableAction);
                }
            }
        }
            break;

        case graphics::resources::Resource::Type::Wheel: {
            myScene = new scenes::WheelScene(name, resource);
            for (auto &action : actions) {
                auto *wheelAction = loadWheelAction(action, name);
                if (wheelAction != nullptr) {
                    myScene->addAction(wheelAction);
                }
            }
        }
            break;
        case graphics::resources::Resource::Type::BGColor: {
            myScene = new scenes::BGColorScene(name, resource);
            for (auto &action : actions) {
                auto *bgColorAction = loadBGColorAction(action, name);
                if (bgColorAction != nullptr) {
                    myScene->addAction(bgColorAction);
                }
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

    myScene->parent(this);
    myScene->verifyIds();
    m_scenes.push_back(myScene);
}

Dimensions Theme::jsonToDimensions(const rapidjson::Value &json, const std::string &name)
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
    dimensionsVariableAssignment(dimensions.size.x, size[0],
                                 name + ".size.x",
                                 Dimensions::kSizeDefault);
    m_calculator.setVariable("width", dimensions.size.x);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.size.y, size[1],
                                 name + ".size.y",
                                 Dimensions::kSizeDefault);
    m_calculator.setVariable("height", dimensions.size.y);


    m_calculator.setHundredPercentByVariableName("screenw");
    dimensionsVariableAssignment(dimensions.position.x, translate[0],
                                 name + ".translate.x",
                                 Dimensions::kPositionDefault);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.position.y, translate[1],
                                 name + ".translate.y",
                                 Dimensions::kPositionDefault);


    m_calculator.setHundredPercentByVariableName("screenw");
    dimensionsVariableAssignment(dimensions.displacement.x, displacement[0],
                                 name + ".displacement.x",
                                 Dimensions::kDisplacementDefault);

    m_calculator.setHundredPercentByVariableName("screenh");
    dimensionsVariableAssignment(dimensions.displacement.y, displacement[1],
                                 name + ".displacement.y",
                                 Dimensions::kDisplacementDefault);

    m_calculator.setHundredPercent(360.0f);
    dimensionsVariableAssignment(dimensions.angle, targetdeg,
                                 name + ".rotation",
                                 Dimensions::kAngleDefault);

    m_calculator.setHundredPercent(1.0f);
    dimensionsVariableAssignment(dimensions.opacity, targetOpacity,
                                 name + ".opacity",
                                 Dimensions::kOpacityDefault);

    return dimensions;
}


void Theme::dimensionsVariableAssignment(float &output, std::string &expression, const std::string &variableName,
                                         float defaultValue)
{
    output = m_calculator.calculate(expression, defaultValue);
    if (output != defaultValue) {
        m_calculator.setVariable(variableName, output);
    }
}


actions::Action *Theme::loadBGColorAction(const rapidjson::Value &action, const std::string &name)
{
    std::string id = filesystem::file::getString(action, "id");
    if (id.empty()) {
        m_debug.error("discarding action without id");
        return nullptr;
    }

    std::string time = filesystem::file::getString(action, "time", "0");
    std::string next = filesystem::file::getString(action, "next", id);

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

    myAction->id(id);
    return myAction;
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

actions::Action *Theme::loadDrawableAction(const rapidjson::Value &action, const std::string &name)
{
    std::string id = filesystem::file::getString(action, "id");
    if (id.empty()) {
        m_debug.error("discarding action without id");
        return nullptr;
    }

    std::string time = filesystem::file::getString(action, "time", "0");
    std::string next = filesystem::file::getString(action, "next", id);

    GLfloat durationMs = convertUnitToNumber(time, 0, ConversionScale::None);

    auto *myAction = new actions::DrawableAction;
    myAction->dimensions(jsonToDimensions(action, name + "." + id));
    myAction->duration(durationMs);
    myAction->resetTime();
    myAction->next(next);

    if (durationMs > 0) {
        myAction->formula(actions::Action::UpdateFormula::Linear);
    }

    myAction->id(id);
    return myAction;
}


void Theme::draw(graphics::textures::Renderer &renderer)
{
    for (auto &scene: m_scenes) {
        scene->draw(renderer);
    }
}

void Theme::update(GLfloat dt, glm::vec4 &bgColor)
{
    for (auto scene : m_scenes) {
        scene->update(dt);
    }

    // reset to resting state so we don't calculate double disposition
    m_indexChanged = false;

    if (m_dying) {
        bool alive = false;
        for (auto scene: m_scenes) {
            if (!scene->died()) {
                alive = true;
                break;
            }
        }

        if (!alive && m_killCallback != nullptr) {
            m_killCallback();
        }
    }
}

actions::Action *Theme::loadWheelAction(const rapidjson::Value &action, const std::string &name)
{
    std::string id = filesystem::file::getString(action, "id");
    if (id.empty()) {
        m_debug.error("discarding action without id");
        return nullptr;
    }


    if (id != "scroll") {
        return loadDrawableAction(action, name);
    }

    // the action is a scrolling action for the wheel scrolling transition
    std::string scroll_time = filesystem::file::getString(action, "scroll_time");
    int durationMs = convertUnitToNumber(scroll_time, 0, ConversionScale::None);

    auto *wheelAction = new actions::WheelAction;
    wheelAction->id(id);
    wheelAction->duration(durationMs);

    return wheelAction;
}


void Theme::loadSystemImages()
{
    m_wheelImageLoader.loadGameSystemsFromDB();
}

void Theme::loadGameImages(const filesystem::database::entity::GameSystem &system)
{
    m_wheelImageLoader.loadGamesFromDB(system);
}


void Theme::drawWheel(graphics::textures::Renderer &renderer, Dimensions selectedPos, glm::vec2 disposition,
                      int selectedIndex)
{
    auto &drawables = getDrawables();

    int count = drawables.size();
    if (count == 0 || (selectedPos.displacement.x == 0.0f && selectedPos.displacement.y == 0.0f)) {
        return;
    }

    int screenWidth = arcade::settings::screen::width();
    int screenHeight = arcade::settings::screen::height();



    // make a copy for the current instance
    auto dim = selectedPos;

    if (dim.position.x < 0 || dim.position.x > screenWidth || dim.position.y < 0 || dim.position.y > screenHeight) {
        return;
    }
    dim.position += disposition;

    if (dim.displacement.x < 0) { dim.displacement.x = -dim.displacement.x; }
    if (dim.displacement.y < 0) { dim.displacement.y = -dim.displacement.y; }


    int index = selectedIndex;
    while (dim.position.y > 0 &&
           dim.position.x > 0) {
        dim.position.y -= dim.displacement.y;
        dim.position.x -= dim.displacement.x;
        index--;
    }

    // make sure that the index is an item in the list and not out of bounds
    // if we only have 1 or 2 items, print those 2 alternately.
    // compensate for the upcoming modulo
    index--;

    if (index < 0) {
        index *= -1;
        index = count - index;
    }

    while (dim.position.x - (dim.size.x / 2.0f) < screenWidth &&
           dim.position.y - (dim.size.y / 2.0f) < screenHeight) {
        index = (index + 1) % count;
        auto drawable = drawables[index];


        graphics::textures::Texture2D tex;
        if (m_wheelImageLoader.get(drawable, tex)) {
            renderer.draw(tex, dim.position, dim.size);
        }

        dim.position += dim.displacement;
    }
}

void Theme::load(const std::string &path, glm::vec4 &bgcolor)
{
    rapidjson::Document d;
    if (!filesystem::file::readJson(path, d)) {
        m_debug.error("failed to read theme file. file is corrupt / nonexistent. discarding.");
        return;
    }

    if (!d.IsObject()) {
        m_debug.error("malformed theme file. document is not an object");
        return;
    }

    std::string jsonRoot = filesystem::path::getPathWithoutFileName(path);

    m_calculator.setVariable("screenw", arcade::settings::screen::width());
    m_calculator.setVariable("screenh", arcade::settings::screen::height());


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
        auto *texResource = new graphics::resources::TextureResource(path, name, this);
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
        auto *bgColorResource = new graphics::resources::BGColorResource(name, bgcolor, this);
        m_resources[name] = bgColorResource;
    }
    else if (type == "wheel") {
        auto *wheelResource = new graphics::resources::WheelResource(name, this);
        m_resources[name] = wheelResource;
    }
    else {
        m_debug.error("resource with unknown type '", type, "'. discarding resource");
    }
}

void Theme::setWheelIndex(int index)
{
    m_wheelIndex = index;
    m_indexChanged = true;
    m_indexDifferential = 0;

    for(auto scene : m_scenes) {
        auto* wscene = dynamic_cast<drawing::scenes::WheelScene*>(scene);
        if(wscene != nullptr) {
            wscene->internalDrawingIndex(index);
        }
    }
}

} // namespace drawing
} // namespace graphics