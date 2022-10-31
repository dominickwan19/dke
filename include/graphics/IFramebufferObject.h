//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_IFramebufferOjbect_h
#define DKE_Graphics_IFramebufferOjbect_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// define an attachable class that provides all information required for a framebuffer to do an attachment
class IFramebufferAttachable {
public:
    virtual ~IFramebufferAttachable() {}

    virtual int getObjectId() = 0;
    virtual int getObjectType() = 0;
    virtual int getAttachment() = 0;
    virtual int getAttachmentTarget() = 0;

    virtual int create() = 0;
    virtual void destroy() = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual bool resize(int32_t width, int32_t height) = 0;

protected:
    IFramebufferAttachable() {}
};

class IFramebufferObject {
public:
    enum FramebufferAttachmentEnum {
        FBO_ATTACHMENT_TEXTURE,
        FBO_ATTACHMENT_RENDERBUFFER
    };

public:
    virtual ~IFramebufferObject() {}

    virtual void create() = 0;
    virtual void destroy() = 0;

    virtual unsigned int attach(std::vector<IFramebufferAttachable*>& colorBuffers, IFramebufferAttachable* depthBuffer, bool checkCompleteness = false) = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void blitToBuffer(uint32_t targetBufferID, uint32_t targetAttachment) = 0;

protected:
    IFramebufferObject() {}
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_IFramebufferOjbect_h
