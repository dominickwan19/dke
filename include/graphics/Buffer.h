//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Buffer_h
#define DKE_Graphics_Buffer_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

/*!
* Buffer manages data between host application to device.
*
* It handles data transfer between host application and device with operations like cacheToDevice()
* and readFromDevice(), which sends/reads in-memory data to/from device. Host application can
* manipulate buffer content with addData(), or retrieve data pointer with getDataPointer() and deal
* with them memory directly.
*/
class Buffer {
public:
    Buffer(uint32_t target, size_t bufferSize);
    virtual ~Buffer();

    int32_t addData(void* data, size_t dataSizeInByte);

    template <typename T>
    int32_t addData(T* data, size_t dataSize)
    {
        int32_t index = (int32_t)m_data.size() / sizeof(T);
        m_data.insert(m_data.end(), (char*)data, (char*)data + dataSize * sizeof(T));
        return index;
    }

    void* getDataPointer();
    size_t getDataSizeInBytes() const;

    virtual void bind(int32_t bindLocation = 0, int32_t componentSize = 0);
    virtual void unbind();
    // virtual void bindBase
    // virtual void bindRange

    virtual void createOnDevice();
    virtual void destroyOnDevice();
    virtual void cacheToDevice();
    virtual void cacheToDevice(char* data, int32_t start, int32_t size);
    virtual void readFromDevice();

    void clearHostData();

    int32_t id() const;
    void setId(int32_t id);

    uint32_t deviceId() const { return m_deviceId; }

    size_t freeSize() const;

protected:
    // Prevent accidentally copying of Buffer.
    Buffer(const Buffer& b) = delete;

protected:
    const size_t m_bufferSize;
    std::vector<char> m_data; // could be better if this is just a pointer to some other memory store
    uint32_t m_id;
    uint32_t m_deviceId;
    uint32_t m_target;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Buffer_h
