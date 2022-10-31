//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_GeometryBuffer_h
#define DKE_Graphics_GeometryBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Buffer.h>
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------
#include <functional>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Renderable;
class GeometryBuffer;

#define DEFAULT_BUFFER_SIZE 4194304

//-------------------------------------------------------------------------------------------------

// the default validator assume renderable has attributes (0=position,1=normal,2=uv)
bool defaultValidateFunction(const GeometryBuffer* gbc, const Geometry* renderable);

//-------------------------------------------------------------------------------------------------

class GeometryBufferCreator {
public:
    virtual ~GeometryBufferCreator() {}

    virtual GeometryBuffer* createBuffer(uint32_t geometryType, uint32_t geometryFlags, const GeometryFormatDescription& gfd, size_t bufferSize) = 0;
};

//-------------------------------------------------------------------------------------------------

/**
  GeometryBuffer manages multiple buffers that are used in the same draw call.
  In OpenGL terms its the multiple VBOs to be used for a draw call (2.0) or VAO+VBO (3.0+).

  Documentation requires update.
 */
class GeometryBuffer {
public:
    GeometryBuffer(uint32_t geometryType, uint32_t flags, const GeometryFormatDescription& gfd, size_t bufferSize = DEFAULT_BUFFER_SIZE /*, uint64_t typeKey = 0xFFFFFFFFFFFFFFFF, uint64_t refMask = 0xFFFFFFFFFFFFFFFF*/);
    virtual ~GeometryBuffer();

    const GeometryFormatDescription getFormatDescription() const;

    virtual void processRenderable(Renderable* renderable);
    void processGeometry(Geometry* geometry);
    bool canFit(Geometry* geometry);
    void setValidateFunction(std::function<bool(const GeometryBuffer*, const Geometry*)> func);
    void setCreateBufferFunction(std::function<Buffer*(uint32_t, size_t)> func);

    virtual bool bind(Geometry::GeometryBufferHandle* handle = nullptr);
    virtual bool unbind();

    virtual void cache();
    virtual void cache(Geometry* g);

    virtual void create();

    int32_t getId() const;
    uint32_t getGeometryType();
    uint32_t flags();

    Buffer* indexBuffer() const { return m_indexBuffer.get(); }
    Buffer* getBuffer(int32_t key, int32_t index);

    size_t size() { return m_defaultBufferSize; }

    void clearHostStaticData();

protected:
    Buffer* createBuffer(int32_t key, uint32_t target);
    Buffer* findBuffer(int32_t key, size_t desiredSize);
    Buffer* getIndexBuffer();
    int32_t makeBufferKey(int32_t bufferKey);

    void addDataFromGeometry(Geometry* geometry);

protected:
    std::vector<std::unique_ptr<Buffer>> m_buffers;
    std::unique_ptr<Buffer> m_indexBuffer;
    GeometryFormatDescription m_formatDescription;
    int32_t m_id;
    uint32_t m_geometryType;
    uint32_t m_flags;
    size_t m_defaultBufferSize;

    std::function<bool(const GeometryBuffer*, const Geometry*)> m_validateFunction; //<! optional function: validate if geometry matches this collection
    std::function<Buffer*(uint32_t, size_t)> m_createBufferFunction; // seems to be better to use a factory method
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_GeometryBuffer_h
