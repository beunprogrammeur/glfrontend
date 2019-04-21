#ifndef GRAPHICS_DRAWING_THEME_H
#define GRAPHICS_DRAWING_THEME_H

#include <vector>
#include <string>

#include "graphics/resources/texture_resource.h"
#include "graphics/textures/renderer.h"

#include "debug/logger.h"

#include <rapidjson/document.h>

#include "graphics/drawing/scenes/bgcolor_scene.h"
#include "graphics/drawing/dimensions.h"


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




    void loadResource(const std::string& jsonRoot, const rapidjson::Value& resource, glm::vec4& bgcolor);
    void loadScene(const rapidjson::Value& scene);
    void loadDrawableAction(scenes::Scene& scene, const rapidjson::Value& action);
    void loadBGColorAction(scenes::Scene& scene, const rapidjson::Value& action);
    GLfloat convertUnitToNumber(const std::string& unit, int defaultValue = INT_MIN, ConversionScale scale = ConversionScale::None);
public:
     Theme();
    ~Theme();

    void load(const std::string& path, glm::vec4& bgcolor);

    void draw(graphics::textures::Renderer& renderer);
    void update(GLfloat dt, glm::vec4& bgColor, Dimensions& wheelDimensions);
};
} // namespace drawing
} // namespace graphics

#endif