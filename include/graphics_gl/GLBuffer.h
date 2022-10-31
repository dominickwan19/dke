//-------------------------------------------------------------------------------------------------
#ifndef GLBuffer_h
#define GLBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Buffer.h>
#include <GL/glew.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------
namespace graphics{
    graphics::Buffer* createGLBuffer(GLuint bufferTarget, size_t bufferSize);
}
//-------------------------------------------------------------------------------------------------
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLBuffer : public graphics::Buffer {
public:
    GLBuffer(GLuint bufferTarget, size_t bufferSize);
    virtual ~GLBuffer();

    virtual void bind(int32_t bindLocation, int32_t componentSize) override;
    virtual void unbind() override;

    virtual void createOnDevice() override;
    virtual void destroyOnDevice() override;
    virtual void cacheToDevice() override;
    virtual void cacheToDevice(char* data, int32_t startPos, int32_t size) override;
    virtual void readFromDevice() override;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // GLBuffer_h
