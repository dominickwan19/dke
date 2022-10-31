//-------------------------------------------------------------------------------------------------
#ifndef GLGeometryBufferCreator_h
#define GLGeometryBufferCreator_h
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLGeometryBufferCreator : public graphics::GeometryBufferCreator {
public:
    GLGeometryBufferCreator();
    virtual graphics::GeometryBuffer* createBuffer(uint32_t geometryType, uint32_t flags, const graphics::GeometryFormatDescription& gfd, size_t bufferSize) override;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // GLGeometryBufferCreator_h