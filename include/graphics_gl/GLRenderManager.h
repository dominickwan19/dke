//-------------------------------------------------------------------------------------------------
#ifndef GLRenderManager_h
#define GLRenderManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderManager.h>
#include <graphics/Renderable.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/Buffer.h>
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

    void initRenderPasses();

    uint64_t lastFrameTime();
    uint64_t lastUpdateTimestamp();

protected:
    virtual void onUpdate() override;
    virtual void onRender() override;

    void updateLastTimestamp();

public:
    //std::unique_ptr<graphics::IFramebufferObject> m_fbo;
    

    /*glm::mat4 m_cameraProjMat;
    glm::mat4 m_cameraViewMat;

    glm::mat4 m_prevTransformMat;
    glm::mat4 m_currTransformMat;

    bool m_doAntialias;
    uint32_t ubo_handle;

    std::vector<glm::vec2> jitterVals;*/

private:
    uint64_t m_lastUpdateTimestamp;
    uint64_t m_lastLastUpdateTimestamp;
};

//-------------------------------------------------------------------------------------------------
}
}
//-------------------------------------------------------------------------------------------------
#endif // GLRenderManager_h
