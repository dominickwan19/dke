//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Geometry_h
#define DKE_Graphics_Geometry_h
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryAttribute.h>
//-------------------------------------------------------------------------------------------------
#include <assert.h>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Defines a maximum number of vertex attributes supported.
// Should make this larger or make it dynamic.
const int32_t MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT = 4;

struct GeometryFormatDescription {
    size_t numAttributes;
    int* componentSizes;
    int* typeSizes;
    // needs to have a list to map each attribute to an attribute pposition

    bool equals(const GeometryFormatDescription* rhs) const
    {
        if (numAttributes != rhs->numAttributes)
            return false;
        for (int i = 0; i < numAttributes; ++i) {
            if (componentSizes[i] != rhs->componentSizes[i])
                return false;
            if (typeSizes[i] != rhs->typeSizes[i])
                return false;
        }
        return true;
    }
};

enum GeometryFlags {
    GEOMETRY_FLAGS_DYNAMIC,
    GEOMETRY_FLAGS_STATIC
};

//-------------------------------------------------------------------------------------------------

// A geometry with all the data that will be submitted to a draw call.
// One geometry has multiple vertex attribte data that should be submitted
// to the draw call, and they are hanlded by generic GeometryAttribute class
// and related functions.
// Note that element index data are handled separately.
// currently manage vertex attributes and element index only, not managing
// uniform data, texture data, ssbo/texture buffer usage etc. These data
// are expected to be managed by Renderable/Material classes.
class Geometry {
public:
    struct GeometryBufferHandle {
        GeometryBufferHandle()
            : geometryBufferId(-1)
            , startPosition(-1)
            , dataCount(0)
            , indexStart(0)
            , indexCount(0)
            , indexBufferId(0)
        {
        }

        int32_t geometryBufferId;
        int32_t startPosition;
        size_t dataCount;

        int32_t indexStart;
        size_t indexCount;
        int32_t indexBufferId;
    };

public:
    Geometry(uint32_t type);
    virtual ~Geometry();

    uint32_t type() const { return m_type; }

    void addIndices(int32_t* indices, size_t n) { m_indices.addData(indices, n); m_bDirty = true; }
    void updateIndices(int32_t* indices, size_t n) { m_indices.updateData(indices, n); m_bDirty = true; }
    const int32_t* getIndices() const { return m_indices.getDataWithType(); }
    size_t getNumIndices() const { return m_indices.getDataCount(); }
    void offsetIndices(int32_t offset);
    int32_t indexOffset() const { return m_indexOffset; }
    void setFlags(GeometryFlags flags);
    GeometryFlags flags();

    template <typename T>
    void addAttribute(int32_t index, int32_t components)
    {
        GeometryAttribute<T>* attribute = new GeometryAttribute<T>(index, components);
        int32_t loc = (int32_t)m_renderableAttributes.size();
        m_renderableAttributes.push_back(attribute);

        assert(attribute->attributeIndex() >= 0 && attribute->attributeIndex() < MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT);
        m_attributeIndexMap[attribute->attributeIndex()] = loc;
        m_bDirty = true;
    }

    template <typename T>
    void addAttributeData(int32_t attributeIndex, T* dataArray, size_t n)
    {
        getAttribute(attributeIndex)->addData(dataArray, n);
        m_bDirty = true;
    }

    template <typename T>
    void updateAttributeData(int32_t attributeIndex, T* dataArray, size_t n)
    {
        getAttribute(attributeIndex)->updateData(dataArray, n);
        m_bDirty = true;
    }

    template <typename T>
    T* getAttributeData(int32_t attributeIndex) const
    {
        auto attr = std::dynamic_pointer_cast<T>(getAttribute(attributeIndex));
        return attr->getDataWithType();
    }

    bool hasAttribute(int32_t attributeIndex) const;
    int32_t getAttributeDataSizeInBytes(int32_t attributeIndex) const;
    int32_t getAttributeDataSize(int32_t attributeIndex) const;
    int32_t getAttributeComponentSize(int32_t attributeIndex) const;
    void* getAttributeData(int32_t attributeIndex) const;
    size_t getAttributeSize() const;
    int32_t getAttributeTypeSizeInBytes(int32_t attributeIndex) const;

    void setTextureId(int32_t textureId) { m_textureId = textureId; m_bDirty = true; }
    int32_t textureId() const { return m_textureId; }

    const GeometryBufferHandle& bufferHandle() const { return m_bufferHandle; }
    void setBufferHandle(const GeometryBufferHandle& bufferHandle);
    void updateBufferIndices(int32_t indexBufferId, int32_t indexStart, size_t indexCount);
    bool isHandleValid() const;
    void invalidateBufferHandle();

    void setDataCount(int32_t count) { m_bufferHandle.dataCount = count; m_bDirty = true; }
    int32_t getDataCount() const;

    // functions for updating
    void update();
    bool isDirty() const;
    void setDirty(bool dirty);

    void clearData();

    GeometryAttributeBase* getAttribute(int32_t attributeIndex) const;

    int32_t id() const { return m_id; }
    void setId(int32_t id) { m_id = id; }

private:
    int m_attributeIndexMap[MAX_GEOMETRY_VERTEX_ATTRIBUTE_COUNT];
    std::vector<GeometryAttributeBase*> m_renderableAttributes; // vertex attribute data for this renderable
    GeometryAttribute<int32_t> m_indices;

    bool m_bDirty;

    // TODO: Consider if we should move renderableBufferHandle out of this class to another mapping structure.
    GeometryBufferHandle m_bufferHandle; // Retrieve the buffers and offsets which contain data of this renderable, could pull it out of this class.
    uint32_t m_type;
    int32_t m_textureId;
    int32_t m_indexOffset;

    int32_t m_id;

    GeometryFlags m_flags;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Geometry_h
