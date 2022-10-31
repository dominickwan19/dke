#include <graphics/MaterialManager.h>
#include <graphics/ITextureObject.h>

namespace dke {
namespace graphics {

TextureManager::~TextureManager()
{
    for (auto itr : m_textureMap)
        delete itr.second;
}

}
}
