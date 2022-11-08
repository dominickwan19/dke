//-------------------------------------------------------------------------------------------------
#ifndef GLFramebufferObject_h
#define GLFramebufferObject_h
//-------------------------------------------------------------------------------------------------
#include <graphics/IFramebufferObject.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLFramebufferObject : public graphics::IFramebufferObject {
public:
    GLFramebufferObject();
    virtual ~GLFramebufferObject();

    virtual void create();
    virtual void destroy();

    virtual uint32_t attach(std::vector<graphics::IFramebufferAttachable*>& colorBuffers, graphics::IFramebufferAttachable* depthBuffer, bool checkCompleteness = false);

    virtual void bind();
    virtual void unbind();

    virtual void blitToBuffer(uint32_t targetBufferID, uint32_t targetAttachment);

private:
    uint32_t m_fboID;
    std::vector<uint32_t> m_activeBuffers;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // GLFramebufferObject_h
