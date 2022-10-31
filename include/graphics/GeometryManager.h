//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_GeometryManager_h
#define DKE_Graphics_GeometryManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Forward Declarations.
class GeometryBufferCreator;
class GeometryBuffer;
class Renderable;
class RenderManager;
class Geometry;

//-------------------------------------------------------------------------------------------------

/**
  GeometryManager manages GeometryBuffers.
 */
class GeometryManager {
public:
    GeometryManager(RenderManager* renderManager);
    virtual ~GeometryManager();

    bool hasBuffer(int32_t index) const;
    GeometryBuffer* getBuffer(int32_t index) const;
    GeometryBuffer* findGeometryBuffer(Geometry* g, uint32_t flags, const GeometryFormatDescription& formatDescription);
    GeometryBuffer* addGeometryBuffer(int32_t geometryType, uint32_t flags, const GeometryFormatDescription& formatDescription);

    void cacheGeometry();
    void cacheGeometry(Geometry* g);

    void setGeometryBufferCreator(GeometryBufferCreator* creator);

    void updateGeometries();

    void deleteBuffers();

    int32_t getGeometryBufferSize();

    void clearHostStaticData();

private:
    RenderManager* m_renderManager;
    std::unique_ptr<GeometryBufferCreator> m_geometryBufferCreator;
    //std::vector<GeometryBuffer*> m_buffers;
    std::map<int32_t, std::unique_ptr<GeometryBuffer>> m_buffers;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_GeometryManager_h
