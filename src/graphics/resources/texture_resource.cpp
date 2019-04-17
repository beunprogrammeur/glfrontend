#include "graphics/resources/texture_resource.h"
#include "filesystem/file.h"
namespace graphics
{
namespace resources
{

bool TextureResource::load()
{
    if(filesystem::file::openTexture(path(), m_texture))
    {
        markLoaded();
    }
    return isLoaded();
}


} // namespace resources
} // namespace graphics
