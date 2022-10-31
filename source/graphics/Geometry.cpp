//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------
#include <assert.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

static int g_geometryId = 0;

Geometry::Geometry(uint32_t type)
    : m_type(type)
    , m_bDirty(false)
    , m_textureId(0)
    , m_indexOffset(0)
    , m_flags(GEOMETRY_FLAGS_DYNAMIC)
{
    m_attributeIndexMap[0] = -1;
    m_attributeIndexMap[1] = -1;
    m_attributeIndexMap[2] = -1;
    m_attributeIndexMap[3] = -1;

    m_id = g_geometryId++;
}

//-------------------------------------------------------------------------------------------------

Geometry::~Geometry()
{
    for (auto* r : m_renderableAttributes)
        delete r;
    m_renderableAttributes.clear();
}

//-------------------------------------------------------------------------------------------------

void Geometry::offsetIndices(int32_t offset)
{
    m_indexOffset = offset;
    //int32_t* indices = m_indices.getDataWithType();
    //for (int i = 0; i < m_indices.getDataCount(); ++i)
    //    indices[i] += offset;
}

//-------------------------------------------------------------------------------------------------

bool Geometry::hasAttribute(int32_t attributeIndex) const
{
    assert(attributeIndex >= 0 && attributeIndex < MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT);
    return m_attributeIndexMap[attributeIndex] != -1;
}

//-------------------------------------------------------------------------------------------------

int32_t Geometry::getAttributeDataSizeInBytes(int32_t attributeIndex) const
{
    GeometryAttributeBase* pAttribute = getAttribute(attributeIndex);
    return pAttribute->getDataCount() * pAttribute->getTypeSizeInBytes() * pAttribute->getNumComponents();
}

//-------------------------------------------------------------------------------------------------

int32_t Geometry::getAttributeDataSize(int32_t attributeIndex) const
{
    return getAttribute(attributeIndex)->getDataCount();
}

//-------------------------------------------------------------------------------------------------

int32_t Geometry::getAttributeComponentSize(int32_t attributeIndex) const
{
    return getAttribute(attributeIndex)->getNumComponents();
}

//-------------------------------------------------------------------------------------------------

void* Geometry::getAttributeData(int32_t attributeIndex) const
{
    return getAttribute(attributeIndex)->getData();
}

//-------------------------------------------------------------------------------------------------

//template <typename T>
//T* Geometry::getAttributeData(int32_t attributeIndex) const
//{
//    return getAttribute(attributeIndex)->getData<T>();
//}

//-------------------------------------------------------------------------------------------------

int32_t Geometry::getAttributeTypeSizeInBytes(int32_t attributeIndex) const
{
    return getAttribute(attributeIndex)->getTypeSizeInBytes();
}

//-------------------------------------------------------------------------------------------------

size_t Geometry::getAttributeSize() const
{
    return m_renderableAttributes.size();
}

//-------------------------------------------------------------------------------------------------

GeometryAttributeBase* Geometry::getAttribute(int32_t attributeIndex) const
{
    assert(attributeIndex >= 0 && attributeIndex < MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT);
    if (attributeIndex < 0 || attributeIndex >= MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT)
        return nullptr;

    int32_t loc = m_attributeIndexMap[attributeIndex];
    return m_renderableAttributes[loc];
}

//-------------------------------------------------------------------------------------------------

void Geometry::setBufferHandle(const GeometryBufferHandle& bufferHandle)
{
    m_bufferHandle = bufferHandle;
    m_bDirty = true;
}

//-------------------------------------------------------------------------------------------------

void Geometry::updateBufferIndices(int32_t indexBufferId, int32_t indexStart, size_t indexCount)
{
    m_bufferHandle.indexStart = indexStart;
    m_bufferHandle.indexBufferId = indexBufferId;
    m_bufferHandle.indexCount = indexCount;
    m_bDirty = true;
}

//-------------------------------------------------------------------------------------------------

bool Geometry::isDirty() const
{
    return m_bDirty;
}

//-------------------------------------------------------------------------------------------------

void Geometry::setDirty(bool dirty)
{
    m_bDirty = dirty;
}

//-------------------------------------------------------------------------------------------------

int32_t Geometry::getDataCount() const
{
    // Number of vertices.

    //if (getNumIndices() != 0)
        //return getNumIndices();

    // Should just return getAttributeDataSize as this should be number of vertex.
	// Get rid of component size later.
    GeometryAttributeBase* pAttribute = getAttribute(0);
    return pAttribute->getDataCount() * pAttribute->getNumComponents();
}

//-------------------------------------------------------------------------------------------------

void Geometry::update()
{
    // get geometry buffer
    // generate new buffer handle
    // if buffer handle changed
    //    append to end of buffer
    // else
    //    simply update data
}

//-------------------------------------------------------------------------------------------------

bool Geometry::isHandleValid() const
{
    auto a = getNumIndices();
    auto b = getDataCount();
    if (m_bufferHandle.geometryBufferId >= 0 && m_bufferHandle.indexCount == getNumIndices() && m_bufferHandle.dataCount == getDataCount())
        // && m_bufferHandle.startPosition != -1
        return true;

    return false;
}

//-------------------------------------------------------------------------------------------------

void Geometry::invalidateBufferHandle()
{
    m_bufferHandle.geometryBufferId = -1;
    m_bufferHandle.dataCount = 0;
    m_bufferHandle.indexCount = 0;
}

//-------------------------------------------------------------------------------------------------

void Geometry::setFlags(GeometryFlags flags)
{
    m_flags = flags;
}

//-------------------------------------------------------------------------------------------------

GeometryFlags Geometry::flags()
{
    return m_flags;
}

//-------------------------------------------------------------------------------------------------

void Geometry::clearData()
{
    for (auto* r : m_renderableAttributes)
        r->clearData();
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
