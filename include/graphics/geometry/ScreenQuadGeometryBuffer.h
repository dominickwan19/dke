//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_ScreenQuadGeometryBuffer_h
#define DKE_Graphics_ScreenQuadGeometryBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class ScreenQuadGeometryBuffer : public graphics::GeometryBuffer {
public:
    ScreenQuadGeometryBuffer(const graphics::GeometryFormatDescription& gfd, uint32_t flags, size_t defaultSize);

    void cache() override;
    bool bind(graphics::Geometry::GeometryBufferHandle* handle = nullptr) override;

private:
    uint64_t m_vaoId;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // DKE_Graphics_ScreenQuadGeometryBuffer_h
