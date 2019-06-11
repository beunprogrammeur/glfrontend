#ifndef GRAPHICS_DRAWING_THEME_H
#define GRAPHICS_DRAWING_THEME_H

#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>
#include <functional>

#include <rapidjson/document.h>


#include "graphics/textures/renderer.h"

#include "graphics/drawing/dimensions.h"
//#include "graphics/drawing/drawable.h"
#include "graphics/drawing/scenes/bgcolor_scene.h"
#include "graphics/drawing/actions/bgcolor_action.h"
#include "graphics/drawing/actions/wheel_action.h"


#include "filesystem/database/entity/entity_loader.h"

#include "math/shuntingyard.h"
#include "debug/logger.h"

namespace graphics {

namespace resources {
// forward declare
class Resource;

// forward declare
class WheelResource;

// forward declare
class BGColorResource;
}

namespace drawing {
class Theme
{
private:
    std::map<std::string, graphics::resources::Resource *> m_resources;
    std::vector<scenes::Scene *> m_scenes;
    debug::Logger m_debug;
    math::shuntingyard::Calculator m_calculator;
    std::vector<filesystem::database::entity::TextureMetaInfo> m_wheelDrawables;
    filesystem::database::entity::EntityLoader m_wheelImageLoader;
    bool m_dying;
    std::function<void()> m_killCallback;

    int m_wheelIndex;
    int m_indexDifferential;
    bool m_indexChanged;

    enum class ConversionScale
    {
        HorizontalPixels,
        VerticalPixels,
        None
    };


    void loadResource(const std::string &jsonRoot, const rapidjson::Value &resource, glm::vec4 &bgcolor);

    void loadScene(const rapidjson::Value &scene);

    actions::Action *loadDrawableAction(const rapidjson::Value &action, const std::string &name);

    actions::Action *loadWheelAction(const rapidjson::Value &action, const std::string &name);

    actions::Action *loadBGColorAction(const rapidjson::Value &action, const std::string &name);

    static GLfloat convertUnitToNumber(const std::string &unit, GLfloat defaultValue = INT_MIN,
                                       ConversionScale scale = ConversionScale::None);

    Dimensions jsonToDimensions(const rapidjson::Value &json, const std::string &name);

    void dimensionsVariableAssignment(float &output, std::string &expression,
                                      const std::string &variableName,
                                      float defaultValue);

public:
    Theme();

    ~Theme();

    void dispose();

    void load(const std::string &path, glm::vec4 &bgcolor);

    void draw(graphics::textures::Renderer &renderer);

    void update(GLfloat dt, glm::vec4 &bgColor);

    inline int getWheelIndex()
    { return m_wheelIndex; }

    inline void scrollUp()
    {
        m_wheelIndex = (m_wheelIndex + 1) % m_wheelDrawables.size();
        m_indexChanged = true;
        m_indexDifferential = 1;
    }

    inline void scrollDown()
    {
        m_wheelIndex--;
        if (m_wheelIndex < 0) { m_wheelIndex = m_wheelDrawables.size() - 1; }
        m_indexChanged = true;
        m_indexDifferential = -1;
    }

    void setWheelIndex(int index);

    inline bool wheelIndexChanged() const
    { return m_indexChanged; }


    inline int indexDifferential() const
    { return m_indexDifferential; }

    inline graphics::resources::Resource *resource(const std::string &id)
    { return m_resources[id]; };

    inline void setDrawables(std::vector<filesystem::database::entity::TextureMetaInfo> &drawables)
    {
        m_wheelDrawables = std::vector<filesystem::database::entity::TextureMetaInfo>(drawables.begin(),
                                                                                      drawables.end());
    }

    inline std::vector<filesystem::database::entity::TextureMetaInfo> &getDrawables()
    { return m_wheelDrawables; };

    void loadSystemImages();

    void loadGameImages(const filesystem::database::entity::GameSystem &system);

    void
    drawWheel(graphics::textures::Renderer &renderer, Dimensions selectedPos, glm::vec2 disposition, int selectedIndex);

    void kill(std::function<void()> callback);
};


} // namespace drawing
} // namespace graphics

#endif
