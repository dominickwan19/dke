//-------------------------------------------------------------------------------------------------
#ifndef DKE_DKEViewerRenderManager_h
#define DKE_DKEViewerRenderManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLRenderManager.h>
//-------------------------------------------------------------------------------------------------

namespace dke {
namespace graphics {
class ITextureObject;
}

namespace graphics_gl {
class GLTextureObject;
}
}
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------


enum ShaderEnum {
    SHADER_SCREEN,
    SHADER_GEOMETRY,
    SHADER_SSAO,
    SHADER_HBAO
};

class DKEViewerRenderManager : public dke::graphics_gl::GLRenderManager
{
public:
    DKEViewerRenderManager();
    ~DKEViewerRenderManager();

    graphics::ITextureObject* getTextureResource(const std::string&& resourceName);

protected:
    virtual void onInitialize() override;
    void initRenderPasses();
    void createRandomTexture();

private:
    graphics_gl::GLTextureObject* m_randomTexture;

    std::unordered_map<std::string, std::unique_ptr<graphics::ITextureObject>> m_textureResourceMap;
};

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_ViewerApp_h

