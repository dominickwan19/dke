//-------------------------------------------------------------------------------------------------
#ifndef GLRenderManager_h
#define GLRenderManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderManager.h>
#include <graphics/Renderable.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/Buffer.h>
#include <graphics/IFramebufferObject.h>
//-------------------------------------------------------------------------------------------------
#include <glm/glm.hpp>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLRenderManager : public graphics::RenderManager {
public:
    GLRenderManager();
    virtual ~GLRenderManager();

    void initialize() override;
    void cacheGeometries() override;
    void reset() override;

    graphics::IFramebufferObject* getFBO();
    graphics::IFramebufferObject* createFBO();

    uint64_t lastFrameTime();
    uint64_t lastUpdateTimestamp();

protected:
    virtual void onUpdate() override;
    virtual void onRender() override;

    void updateLastTimestamp();

public:
    std::unique_ptr<graphics::IFramebufferObject> m_fbo;

private:
    uint64_t m_lastUpdateTimestamp;
    uint64_t m_lastLastUpdateTimestamp;
};

//-------------------------------------------------------------------------------------------------
}
}
//-------------------------------------------------------------------------------------------------
#endif // GLRenderManager_h
