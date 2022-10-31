//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
//-------------------------------------------------------------------------------------------------
#include <algorithm>
#include <assert.h>
#include <graphics/Renderable.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

bool defaultValidateFunction(const GeometryBuffer* gbc, const Geometry* renderable)
{
    // When adding a renderable, make sure it's format is the same as this collection.
    // Check for (1) number of attributes match and...
    size_t numRenderableAttributes = renderable->getAttributeSize();
    if (numRenderableAttributes != gbc->getFormatDescription().numAttributes) {
        printf("GBC Validate: number of attributes not matched.\n");
        return false;
    }

    //// ...(2) all attirubte indices match and...
    //for (auto f : gbc->getFormatDescription()) {
    //    int32_t attributeIndex = f.first;
    //    if (!renderable->hasAttribute(attributeIndex)) {
    //        printf("GBC validate: attribute index %d not matched.\n", attributeIndex);
    //        return false;
    //    }
    //}

    //if (numRenderableAttributes != 3) {
    //    printf("GBC validate: number of attributes does not fit into default validate function.\n");
    //    return false;
    //}

    //// ...(3) the renderable contains valid data
    //bool sameSize = renderable->getAttributeDataSize(0) == renderable->getAttributeDataSize(1) && renderable->getAttributeDataSize(0) == renderable->getAttributeDataSize(2);
    //if (!sameSize) {
    //    printf("GBC validate: attribute data size incorrect.\n");
    //    return false;
    //}

    return true;
}

//-------------------------------------------------------------------------------------------------

GeometryBuffer::GeometryBuffer(uint32_t geometryType, uint32_t flags, const GeometryFormatDescription& gfd, size_t bufferSize /*, uint64_t typeKey, uint64_t refMask*/)
    : m_indexBuffer(nullptr)
    , m_validateFunction(defaultValidateFunction)
    , m_createBufferFunction(nullptr)
    , m_defaultBufferSize(bufferSize)
    , m_geometryType(geometryType)
    , m_flags(flags)
{
    static int32_t id = 0;
    m_id = id;
    id++;

    m_formatDescription.numAttributes = gfd.numAttributes;
    m_formatDescription.componentSizes = new int32_t[gfd.numAttributes];
    m_formatDescription.typeSizes = new int32_t[gfd.numAttributes];
    for (size_t i = 0; i < gfd.numAttributes; i++) {
        m_formatDescription.componentSizes[i] = gfd.componentSizes[i];
        m_formatDescription.typeSizes[i] = gfd.typeSizes[i];
    }
}

//-------------------------------------------------------------------------------------------------

GeometryBuffer::~GeometryBuffer()
{
    delete[] m_formatDescription.componentSizes;
    delete[] m_formatDescription.typeSizes;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::create()
{
    for (int i = 0; i < m_formatDescription.numAttributes; i++)
        createBuffer(i, 0x8892);
}

//-------------------------------------------------------------------------------------------------

//void GeometryBuffer::addFormatDescription(int32_t key, int32_t componentSize, int32_t typeSizeInBytes)
//{
//    FormatDescriptionStruct fd { componentSize, typeSizeInBytes };
//    //m_formatDescription.emplace(key, fd);
//}

//-------------------------------------------------------------------------------------------------

const GeometryFormatDescription GeometryBuffer::getFormatDescription() const
{
    return m_formatDescription;
}

//-------------------------------------------------------------------------------------------------

Buffer* GeometryBuffer::createBuffer(int32_t key, uint32_t target)
{
    int32_t maxComponentSize = 0;
    for (size_t i = 0; i < m_formatDescription.numAttributes; i++)
        maxComponentSize = std::max<int32_t>(maxComponentSize, m_formatDescription.componentSizes[i]);

    int32_t componentSize = m_formatDescription.componentSizes[key];
    int32_t bufferTypeByteSize = m_formatDescription.typeSizes[key];

    // generate a buffer with size defined for this collection.  Since not all buffers
    // have the same number of components, we will smaller the size of buffers with
    // less components proportionally (hence the * componentSize / maxComponentSize term).

    Buffer* buffer;
    if (m_createBufferFunction != nullptr)
        // could use a template or subclass this to create the concrete buffer class?
        buffer = m_createBufferFunction(target, m_defaultBufferSize * componentSize / maxComponentSize);
    else
        // why should we leave this flow here, any use beyond testing purpose?
        buffer = new Buffer(target, m_defaultBufferSize * componentSize / maxComponentSize);
    buffer->setId(makeBufferKey((int32_t)m_buffers.size())); // find a better way to generate ID
    buffer->createOnDevice();

    m_buffers.emplace_back(buffer);
    return buffer;
}

//-------------------------------------------------------------------------------------------------

Buffer* GeometryBuffer::getIndexBuffer()
{
    if (!m_indexBuffer) {
        int32_t componentSize = 1;
        int32_t bufferTypeByteSize = sizeof(int32_t);

        // TODO: change this hardcoded literal - 0x8893 is GL_ELEMENT_ARRAY_BUFFER
        if (m_createBufferFunction != nullptr)
            m_indexBuffer.reset(m_createBufferFunction(0x8893, m_defaultBufferSize));
        else
            m_indexBuffer.reset(new Buffer(0x8893, m_defaultBufferSize));
        //m_indexBuffer->setId(0);
        m_indexBuffer->createOnDevice();
    }
    return m_indexBuffer.get();
}

//-------------------------------------------------------------------------------------------------

//void GeometryBufferCollection::addBuffer(int32_t key, Buffer* buffer)
//{
//    if (m_bufferMap.find(key) == m_bufferMap.end())
//        m_bufferMap.emplace(key, std::vector<Buffer*>());
//}

//-------------------------------------------------------------------------------------------------

Buffer* GeometryBuffer::getBuffer(int32_t key, int32_t index)
{
    if (index >= 0 && index < m_buffers.size())
        return m_buffers.at(index).get();

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

Buffer* GeometryBuffer::findBuffer(int32_t key, size_t desiredSize)
{
    int index = 0;
    for (const auto& b : m_buffers) {
        if (b->freeSize() >= desiredSize && index == key)
            return b.get();
        index++;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::processRenderable(Renderable* renderable)
{
    for (Geometry* g : renderable->geometries())
        addDataFromGeometry(g);
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::processGeometry(Geometry* geometry)
{
    addDataFromGeometry(geometry);
}

//-------------------------------------------------------------------------------------------------

bool GeometryBuffer::canFit(Geometry* geometry)
{
    // All attributes can fit in all buffers.
    for (int i = 0; i < m_formatDescription.numAttributes; i++) {
        if (m_buffers[i]->freeSize() < geometry->getAttributeDataSizeInBytes(i))
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::addDataFromGeometry(Geometry* geometry)
{
    // debug only
    if (m_validateFunction) {
        if (!m_validateFunction(this, geometry)) {
            assert(false && "Geometry validate failed!");
            return;
        }
    }

    Geometry::GeometryBufferHandle bufferHandle;
    bufferHandle.geometryBufferId = m_id;

    int32_t currentIndex = 0;
    int32_t vertexPositionOffset = 0;

    // 1. for each attribute, find a suitable buffer, and add corresponding data to buffer from geometry
    // 2. after inserting, update the buffer position and id back to geometry so that the geometry knows how to locate buffers in the device
    //for (auto f : m_formatDescription) {
    for (int32_t i = 0; i < m_formatDescription.numAttributes; i++) {
        int32_t attributeIndex = i;
        GeometryAttributeBase* pAttribute = geometry->getAttribute(attributeIndex);
        if (!pAttribute)
            continue;

        size_t attributeSizeInBytes = pAttribute->getDataCount() * pAttribute->getTypeSizeInBytes() * pAttribute->getNumComponents();
        Buffer* buffer = findBuffer(attributeIndex, attributeSizeInBytes);

        // We should always have a buffer for a given geometry, but a geometry can be too large to
        // fit into the buffer.
        // For now we reject the geometry if it cannot fit.
        if (!buffer) {
            printf("geometry cannot fit into buffer!\n");
            return;
        }

        int32_t bufferPositionIndex = buffer->addData(pAttribute->getData(), attributeSizeInBytes);
        if (!currentIndex) {
            vertexPositionOffset = bufferPositionIndex;

            int32_t bufferId = buffer->id();

            bufferHandle.startPosition = bufferPositionIndex;
            //bufferHandle.bufferIds[currentIndex] = bufferId;
            bufferHandle.dataCount = pAttribute->getDataCount() * pAttribute->getNumComponents();

            bufferHandle.indexCount = geometry->getNumIndices();
            bufferHandle.indexStart = 0;

            // bug here: in WDERenderableCommandGenerator, the buffer data count is based on component size of attirbuteIndex0
            // but without this code the bufferHandle is the last attributeIndex. Should only use one bufferHandle for each geometry.
        }

        currentIndex++;
    }

    if (geometry->getNumIndices() > 0) {
        // handle index
        Buffer* buffer = getIndexBuffer();

        // needs to offset for GLES
        geometry->offsetIndices(vertexPositionOffset / sizeof(float) / 3);
        std::vector<int32_t> indices(geometry->getIndices(), geometry->getIndices() + geometry->getNumIndices());
        for (int i = 0; i < indices.size(); ++i) {
            if (indices[i] != -1 && indices[i] != std::numeric_limits<uint32_t>::max())
                indices[i] += geometry->indexOffset();
        }
        int32_t bufferPositionindex = buffer->addData(indices.data(), indices.size());

        // TODO: and need to record base vertex for GL

        bufferHandle.indexStart = bufferPositionindex;
        bufferHandle.indexBufferId = buffer->id();
        bufferHandle.indexCount = geometry->getNumIndices();

        geometry->updateBufferIndices(buffer->id(), bufferPositionindex, geometry->getNumIndices());
    }

    geometry->setBufferHandle(bufferHandle);
}

//-------------------------------------------------------------------------------------------------

bool GeometryBuffer::bind(Geometry::GeometryBufferHandle* handle)
{
    // if 2.0, traverse buffers and bind
    //for (auto info : attributeToLocMap) {
    //    // TODO: if the collection knows about buffer (vbo) ids and format structure (as if vao is used), we can save some data in buffer handles which will be stored in each renderablre
    //    int32_t attributeIndex = info.first;
    //    int32_t bindLocation = info.second;

    //    //FormatDescriptionStruct fd = m_formatDescription[attributeIndex];

    //    // with attirbute index we still need to know which buffer we are binding
    //    //getBuffer(attributeIndex, handle->bufferIds[attributeIndex])- >bind(bindLocation, fd.numComponents);
    //    //printf(" - binding attribute: %d, start %d size %zd\n", handle->bufferIds[attributeIndex], handle->startPositions[attributeIndex], handle->dataCount[attributeIndex]);
    //}

    //printf(" - binding index: %d, start %d size %zd\n", handle->indexBufferId, handle->indexStart, handle->indexCount);

    // if 3.3 or above, use VAO

    return true;
}

//-------------------------------------------------------------------------------------------------

bool GeometryBuffer::unbind()
{
    // if 2.0, traverse buffers and unbind
    //for (auto val : m_bufferMap) {
    //val.second->unbind();
    //}

    // if 3.3 or above, unbind vAo
    return true;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::cache()
{
    for (const auto& b : m_buffers)
        b->cacheToDevice();
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::cache(Geometry* geometry)
{
    if (!geometry->isHandleValid()) {
        addDataFromGeometry(geometry);
        return;
    }

    int32_t vertexPositionOffset = 0;

    size_t baseAttributeComponentSize = geometry->getAttributeComponentSize(0);

    for (int32_t i = 0; i < m_formatDescription.numAttributes; i++) {
        int32_t attributeIndex = i;
        GeometryAttributeBase* pAttribute = geometry->getAttribute(attributeIndex);
        if (!pAttribute)
            continue;

        Buffer* buffer = getBuffer(0, attributeIndex);
        int32_t startPos = geometry->bufferHandle().startPosition;

        if (i == 0)
            vertexPositionOffset = startPos;

        startPos = (int32_t) geometry->bufferHandle().startPosition / (int32_t) baseAttributeComponentSize * (int32_t) pAttribute->getNumComponents();
        int32_t size = pAttribute->getDataCount() * pAttribute->getTypeSizeInBytes() * pAttribute->getNumComponents();
        if (void* pData = pAttribute->getData())
            buffer->cacheToDevice((char*)pData, startPos, size);
    }

    if (geometry->getNumIndices() > 0) {
        // handle index
        Buffer* buffer = getIndexBuffer();

        geometry->offsetIndices(geometry->bufferHandle().startPosition / sizeof(float) / 3);
        std::vector<int32_t> indices(geometry->getIndices(), geometry->getIndices() + geometry->getNumIndices());
        for (size_t i = 0; i < indices.size(); ++i)
            indices[i] += geometry->indexOffset();

        //g->offsetIndices(vertexPositionOffset);
        buffer->cacheToDevice((char*)indices.data(), geometry->bufferHandle().indexStart * sizeof(int32_t), (int32_t) geometry->bufferHandle().indexCount * sizeof(int32_t));
    }
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::setValidateFunction(std::function<bool(const GeometryBuffer*, const Geometry*)> func)
{
    m_validateFunction = func;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::setCreateBufferFunction(std::function<Buffer*(uint32_t, size_t)> func)
{
    m_createBufferFunction = func;
}

//-------------------------------------------------------------------------------------------------

int32_t GeometryBuffer::getId() const
{
    return m_id;
}

//-------------------------------------------------------------------------------------------------

uint32_t GeometryBuffer::getGeometryType()
{
    return m_geometryType;
}

//-------------------------------------------------------------------------------------------------

uint32_t GeometryBuffer::flags()
{
    return m_flags;
}

//-------------------------------------------------------------------------------------------------

int32_t GeometryBuffer::makeBufferKey(int32_t bufferKey)
{
    return bufferKey;
}

//-------------------------------------------------------------------------------------------------

void GeometryBuffer::clearHostStaticData()
{
    if (m_flags == GEOMETRY_FLAGS_STATIC) {
        for (const auto& b : m_buffers) {
            b->clearHostData();
        }
        if (m_indexBuffer)
            m_indexBuffer->clearHostData();
    }
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
