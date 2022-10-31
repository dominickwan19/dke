//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLBuffer.h>
//-------------------------------------------------------------------------------------------------
#include <algorithm>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

namespace graphics {
Buffer* createGLBuffer(GLuint bufferTarget, size_t bufferSize)
{
    return new graphics_gl::GLBuffer(bufferTarget, bufferSize);
}
}
//-------------------------------------------------------------------------------------------------
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

GLBuffer::GLBuffer(GLuint bufferTarget, size_t bufferSize)
    : Buffer(bufferTarget, bufferSize)
{
}

//-------------------------------------------------------------------------------------------------

GLBuffer::~GLBuffer()
{
    destroyOnDevice();
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::bind(int32_t bindLocation, int32_t componentSize)
{
    glBindBuffer(m_target, m_deviceId);
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::unbind()
{
    glBindBuffer(m_target, 0);
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::createOnDevice()
{
    glGenBuffers(1, &m_deviceId);

    bind(0, 0);

    // We first create a buffer with a fixed size, given by bufferSize parameter.
    // from constructor.
    glBufferData(m_target, m_bufferSize, nullptr, GL_STATIC_DRAW);
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::destroyOnDevice()
{
    glDeleteBuffers(1, &m_deviceId);
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::cacheToDevice()
{
    if (getDataSizeInBytes() == 0)
        return;
		
    bind(0, 0);

    // then place actual data into the created buffer.
    glBufferSubData(m_target, 0, getDataSizeInBytes(), getDataPointer());

    // after caching we can clear memory occupied - they are now on GPU.
    //m_data.resize(0);
    //m_data.shrink_to_fit();
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::cacheToDevice(char* data, int32_t startPos, int32_t size)
{
    bind(0, 0);

    if (startPos + size > m_data.size())
        m_data.resize(std::max<int32_t>((int32_t)m_data.size(), startPos) + size);

    std::copy(data, data + size, m_data.begin() + startPos);

    glBufferSubData(m_target, startPos, size, m_data.data() + startPos);
}

//-------------------------------------------------------------------------------------------------

void GLBuffer::readFromDevice()
{
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
