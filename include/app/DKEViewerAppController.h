//-------------------------------------------------------------------------------------------------
#ifndef DKE_DKEViewerAppController_h
#define DKE_DKEViewerAppController_h
//-------------------------------------------------------------------------------------------------
#include <app/AppController.h>
#include <cstdint>
#include <memory>
//-------------------------------------------------------------------------------------------------

namespace dke{
namespace graphics{
    class Camera;
    class RenderManager;
}
}

//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

class DKEViewerAppController : public AppController
{
public:
    DKEViewerAppController(graphics::RenderManager* renderManager);
    ~DKEViewerAppController();

    void initialize() override;
    void finalize() override;

    void update() override;
    void render() override;

    void onMouseLeftDown(const MouseEventData&& eventData) override;
    void onMouseLeftUp(const MouseEventData&& eventData) override;
    void onMouseMiddleDown(const MouseEventData&& eventData) override;
    void onMouseMiddleUp(const MouseEventData&& eventData) override;
    void onMouseRightDown(const MouseEventData&& eventData) override;
    void onMouseRightUp(const MouseEventData&& eventData) override;
    void onMouseMove(const MouseEventData&& eventData) override;
    void onMouseWheel(const MouseEventData&& eventData) override; 

    void onWindowResize(int width, int height) override;

private:
    std::unique_ptr<graphics::Camera> m_camera;
    std::unique_ptr<graphics::RenderManager> m_renderManager;
};

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_ViewerApp_h

