//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_GeometryAttribute_h
#define DKE_Graphics_GeometryAttribute_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Describe the format of a vertex attribute, defined by
// number of components and byte size of a single element.
// Concrete type could be omitted here because the APIs
// will be working with pointers.
struct FormatDescriptionStruct {
    int32_t numComponents;
    int32_t typeSizeInBytes;
};

//-------------------------------------------------------------------------------------------------

// Interface of geometry attribute.
class GeometryAttributeBase {
public:
    virtual ~GeometryAttributeBase() = default;

    virtual int32_t attributeIndex() const = 0;

    virtual void* getData() = 0;
    virtual int32_t getDataCount() const = 0;
    virtual int8_t getNumComponents() const = 0;
    virtual int32_t getTypeSizeInBytes() const = 0;

    virtual void addData(void* data, size_t dataSize) = 0;
    virtual void updateData(void* data, size_t dataSize) = 0;
    virtual void clearData() = 0;
};

//-------------------------------------------------------------------------------------------------

// Geometry Attribute holds an array of data which will be used
// as vertex attribute for draw calls.
template <typename T>
class GeometryAttribute : public GeometryAttributeBase {
public:
    GeometryAttribute(int32_t attributeIndex = 0, int32_t components = 1)
        : m_numComponents(components)
        , m_attributeIndex(attributeIndex)
        , m_dataSize(0)
    {
    }

    virtual int32_t attributeIndex() const override { return m_attributeIndex; }

    virtual void* getData() override { return m_data.data(); }
    virtual int32_t getDataCount() const override { return (int32_t)m_dataSize / m_numComponents; }
    virtual int8_t getNumComponents() const override { return m_numComponents; }
    virtual int32_t getTypeSizeInBytes() const override { return sizeof(T); }

    virtual void addData(void* data, size_t dataSize) override { addData_((T*)data, dataSize); }
    virtual void updateData(void* data, size_t dataSize) override { updateData_((T*)data, dataSize); }
    virtual void clearData() override { m_data.clear(); m_data.shrink_to_fit(); }

    T* getDataWithType() { return m_data.data(); }
    const T* getDataWithType() const { return m_data.data(); }

private:
    void addData_(T* data, size_t dataSize)
    {
        /*size_t start = m_data.size();
        m_data.resize(start + dataSize);
        memcpy(m_data.data() + start, data, dataSize);*/
        m_data.insert(m_data.end(), data, data + dataSize);
        m_dataSize = m_data.size();
    }
    void updateData_(T* data, size_t dataSize)
    {
        // should have a start index here
        // also debug to check if incoming data exceed m_data size.
        m_data.resize(dataSize);
        m_dataSize = dataSize;
        for (int i = 0; i < dataSize; i++)
            m_data[i] = data[i];
    }

    std::vector<T> m_data;
    size_t m_dataSize;
    int32_t m_numComponents;
    int32_t m_attributeIndex;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_GeometryAttribute_h
