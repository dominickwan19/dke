//-------------------------------------------------------------------------------------------------
#ifndef GLFramebufferObject_h
#define GLFramebufferObject_h
//-------------------------------------------------------------------------------------------------
#include <core/ex_core.h>
#include <graphics/IFramebufferObject.h>
//-------------------------------------------------------------------------------------------------

class GLFramebufferObject : public exgraphics::IFramebufferObject {
public:
    GLFramebufferObject();
    virtual ~GLFramebufferObject();

    virtual void create();
    virtual void destroy();

    virtual uint32_t attach(std::vector<exgraphics::IFramebufferAttachable*>& colorBuffers, exgraphics::IFramebufferAttachable* depthBuffer, bool checkCompleteness = false);

    virtual void bind();
    virtual void unbind();

    virtual void blitToBuffer(uint32_t targetBufferID, uint32_t targetAttachment);

private:
    uint32_t m_fboID;
    std::vector<uint32_t> m_activeBuffers;
};

//-------------------------------------------------------------------------------------------------

#endif // GLFramebufferObject_h
