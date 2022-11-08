//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_ScreenQuadGeometry_h
#define DKE_Graphics_ScreenQuadGeometry_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class ScreenQuadGeometry : public graphics::Geometry {
private:
    enum {
        ATTR_POSITION = 0
    };

public:
    ScreenQuadGeometry();

    inline void addPosition(float* data, size_t n) { addAttributeData<float>(ATTR_POSITION, data, n); }
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // DKE_Graphics_ScreenQuadGeometry_h
