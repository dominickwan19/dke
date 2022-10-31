//-------------------------------------------------------------------------------------------------
#ifndef ScreenQuadGeometryBuffer_h
#define ScreenQuadGeometryBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
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
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // ScreenQuadGeometryBuffer_h
