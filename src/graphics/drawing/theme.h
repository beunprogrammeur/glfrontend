#ifndef GRAPHICS_DRAWING_THEME_H
#define GRAPHICS_DRAWING_THEME_H

#include <vector>
#include <string>

#include "graphics/resources/texture_resource.h"
#include "graphics/textures/renderer.h"

#include "debug/logger.h"

#include <rapidjson/document.h>

#include "graphics/drawing/scene.h"


namespace graphics
{
namespace drawing
{
class Theme
{
private:
    std::map<std::string, graphics::resources::TextureResource*> m_textures;
    std::vector<Scene> m_scenes;
    debug::Logger m_debug;

    enum class ConversionScale
    {
        HorizontalPixels,
        VerticalPixels,
        None
    };


    void loadResource(const std::string& jsonRoot, const rapidjson::Value& resource);
    void loadScene(const rapidjson::Value& scene);
    void loadAction(Scene& scene, const rapidjson::Value& action);
    int convertUnitToNumber(const std::string& unit, int defaultValue = INT_MIN, ConversionScale scale = ConversionScale::None);
public:
     Theme();
    ~Theme();

    void load(const std::string& path);

    void draw(graphics::textures::Renderer& renderer);
    void update(GLfloat dt);
};
} // namespace drawing
} // namespace graphics

#endif