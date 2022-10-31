//-------------------------------------------------------------------------------------------------
#ifndef DKE_AppController_h
#define DKE_AppController_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

struct MouseEventData
{
    int32_t x;
    int32_t y;
    int32_t mods;
};

class AppController
{
public:
    virtual void initialize();
    virtual void finalize();

    virtual void update();
    virtual void render();

    virtual void onMouseLeftDown(const MouseEventData&& eventData);
    virtual void onMouseLeftUp(const MouseEventData&& eventData);
    virtual void onMouseMiddleDown(const MouseEventData&& eventData);
    virtual void onMouseMiddleUp(const MouseEventData&& eventData);
    virtual void onMouseRightDown(const MouseEventData&& eventData);
    virtual void onMouseRightUp(const MouseEventData&& eventData);
    virtual void onMouseMove(const MouseEventData&& eventData);
    virtual void onMouseWheel(const MouseEventData&& eventData);

private:
};

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_ViewerApp_h

