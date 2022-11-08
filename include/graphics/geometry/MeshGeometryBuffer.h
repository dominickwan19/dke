//-------------------------------------------------------------------------------------------------
#ifndef MeshGeometryBuffer_h
#define MeshGeometryBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class MeshGeometryBuffer : public graphics::GeometryBuffer {
public:
    MeshGeometryBuffer(const graphics::GeometryFormatDescription& gfd, uint32_t flags, size_t defaultSize);

    void cache() override;
    bool bind(graphics::Geometry::GeometryBufferHandle* handle = nullptr) override;

private:
    uint64_t m_vaoId;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // MeshGeometryBuffer_h
