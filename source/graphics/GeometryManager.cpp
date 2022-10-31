//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryManager.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
#include <graphics/RenderManager.h>
#include <graphics/Renderable.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

GeometryManager::GeometryManager(RenderManager* renderManager)
    : m_geometryBufferCreator(nullptr)
    , m_renderManager(renderManager)
{
}

//-------------------------------------------------------------------------------------------------

GeometryManager::~GeometryManager()
{
}

//-------------------------------------------------------------------------------------------------

bool GeometryManager::hasBuffer(int32_t index) const
{
    return m_buffers.find(index) != m_buffers.end();
}

//-------------------------------------------------------------------------------------------------

GeometryBuffer* GeometryManager::getBuffer(int32_t index) const
{
    if (hasBuffer(index))
        return m_buffers.at(index).get();
    else {
        printf("GeometryManager::getBuffer() NOT FOUND index: %d\n", index);
        return nullptr;
    }
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::cacheGeometry()
{
    for (const auto& b : m_buffers)
        b.second->cache();
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::cacheGeometry(Geometry* g)
{
    GeometryBuffer* gb = nullptr;
    // Geometry has valid handle and has a valid buffer,
    // and is updating its content in the buffer.
    if (g->isHandleValid() && hasBuffer(g->bufferHandle().geometryBufferId)) {
        gb = getBuffer(g->bufferHandle().geometryBufferId);
    } else {
        // Geometry is new, or the buffer handle is not valid.
        // Create as new geometry.
        size_t numAttributes = g->getAttributeSize();
        GeometryFormatDescription gfd;
        gfd.numAttributes = numAttributes;
        gfd.componentSizes = new int32_t[numAttributes];
        gfd.typeSizes = new int32_t[numAttributes];
        for (int32_t i = 0; i < numAttributes; i++) {
            GeometryAttributeBase* pAttribute = g->getAttribute(i);
            int32_t typeSize = pAttribute->getTypeSizeInBytes();
            int32_t componentSize = pAttribute->getNumComponents();
            gfd.componentSizes[i] = componentSize;
            gfd.typeSizes[i] = typeSize;
        }

        gb = findGeometryBuffer(g, g->flags(), gfd);
        if (!gb || !gb->canFit(g))
            gb = addGeometryBuffer(g->type(), g->flags(), gfd);

        // TODO: Handle it properly
        delete[] gfd.componentSizes;
        delete[] gfd.typeSizes;
    }

    if (gb)
        gb->cache(g);
}

//-------------------------------------------------------------------------------------------------

GeometryBuffer* GeometryManager::findGeometryBuffer(Geometry* g, uint32_t flags, const GeometryFormatDescription& formatDescription)
{
    for (const auto& buffer : m_buffers) {
        if (buffer.second->getGeometryType() == g->type()
            && buffer.second->flags() == g->flags()
            && buffer.second->getFormatDescription().equals(&formatDescription)
            && buffer.second->canFit(g)) {
            return buffer.second.get();
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

GeometryBuffer* GeometryManager::addGeometryBuffer(int geometryType, uint32_t flags, const GeometryFormatDescription& formatDescription)
{
    // should give user a way to customize this value
    int geometryBufferSize = getGeometryBufferSize();

    GeometryBuffer* geometryBuffer = nullptr;
    if (m_geometryBufferCreator)
        geometryBuffer = m_geometryBufferCreator->createBuffer(geometryType, flags, formatDescription, geometryBufferSize);
    else
        geometryBuffer = new GeometryBuffer(geometryType, flags, formatDescription);

    if (geometryBuffer)
        m_buffers.emplace(geometryBuffer->getId(), geometryBuffer);

    return geometryBuffer;
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::setGeometryBufferCreator(GeometryBufferCreator* creator)
{
    m_geometryBufferCreator.reset(creator);
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::updateGeometries()
{
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::deleteBuffers()
{
    m_buffers.clear();
}

//-------------------------------------------------------------------------------------------------

int32_t GeometryManager::getGeometryBufferSize()
{
    // we should have a better way to find out the geometry buffer size for
    // a given type of geometry.
    int32_t geometryBufferSize = DEFAULT_BUFFER_SIZE; // default buffer size is 4mb. Perhaps we should let api users specify this too.
    int32_t maxSize = m_renderManager->renderCapabilities().maxElementIndex * sizeof(float) * 3;
    if (maxSize > std::numeric_limits<int32_t>::max())
        maxSize = std::numeric_limits<int32_t>::max();
    geometryBufferSize = std::min<int32_t>(maxSize, geometryBufferSize);
    return geometryBufferSize;
}

//-------------------------------------------------------------------------------------------------

void GeometryManager::clearHostStaticData()
{
    for (const auto& b : m_buffers) {
        b.second->clearHostStaticData();
    }
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
