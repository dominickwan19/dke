//-------------------------------------------------------------------------------------------------
#ifndef DKE_SSAOPass_h
#define DKE_SSAOPass_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Colour.h>
#include <graphics/framegraph/RenderPass.h>
#include <graphics/framegraph/Resource.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics_gl/GLTextureObject.h>
#include <graphics_gl/GeometryPass.h>
#include <app/DKEViewerRenderManager.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

namespace graphics {
class Renderable;
};

struct SSAOPassData {
    dke::graphics::Texture2DResource* inputColour[2];
    dke::graphics::Texture2DResource* outputColor[1];
    dke::graphics::Texture2DResource* outputDepth;
};

//-------------------------------------------------------------------------------------------------

class SSAOPass : public dke::graphics::RenderPass<SSAOPassData> {
public:
    SSAOPass(int32_t index, dke::graphics::RenderManager* renderManager, dke::graphics::Renderable* screenQuadRenderable);
    ~SSAOPass();

    DKEViewerRenderManager* getRenderManager();

protected:
    virtual void setup(dke::graphics::RenderPassBuilder& builder) override;
    virtual void execute(const dke::graphics::Camera* camera, const dke::graphics::RenderOptions* renderOptions) override;

private:
    dke::graphics::RenderManager* m_renderManager;
    dke::graphics::Renderable* m_screenQuadRenderable;
};

//-------------------------------------------------------------------------------------------------
} // dke
//-------------------------------------------------------------------------------------------------

#endif // DKE_SSAOPass_h
