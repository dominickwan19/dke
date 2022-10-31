//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderOptions_h
#define DKE_Graphics_RenderOptions_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Colour.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class RenderOptions {
public:
    RenderOptions()
        : backgroundColour(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }

    Colour4 backgroundColour;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderOptions_h