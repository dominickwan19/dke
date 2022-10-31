//-------------------------------------------------------------------------------------------------
#include <rendering/gles/GLFramebufferObject.h>
//-------------------------------------------------------------------------------------------------
#include <ex_rendering_platform.h>
//-------------------------------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include <string.h>
//-------------------------------------------------------------------------------------------------

using namespace exgraphics;

#define PRINT_GL_ERROR(msg) \
    { GLenum err = glGetError(); if (err != GL_NO_ERROR) printf(msg, err); }

//-------------------------------------------------------------------------------------------------

GLFramebufferObject::GLFramebufferObject()
    : m_fboID(0)
{
}

//-------------------------------------------------------------------------------------------------

GLFramebufferObject::~GLFramebufferObject()
{
    destroy();
}

//-------------------------------------------------------------------------------------------------

void GLFramebufferObject::create()
{
    if (m_fboID == 0)
        glGenFramebuffers(1, &m_fboID);
}

//-------------------------------------------------------------------------------------------------

void GLFramebufferObject::destroy()
{
    glDeleteFramebuffers(1, &m_fboID);
}

//-------------------------------------------------------------------------------------------------

uint32_t GLFramebufferObject::attach(std::vector<IFramebufferAttachable*>& colorBuffers, IFramebufferAttachable* depthBuffer, bool checkCompleteness)
{
    //bind(;)
    // the bind function checks for activeBuffers which is no t
    // what we want here
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

    // reset the state of active buffers
    m_activeBuffers.clear();
    m_activeBuffers.resize(colorBuffers.size());

    for (size_t i = 0; i < colorBuffers.size(); ++i) {
        if (!colorBuffers[i])
            continue;

        if (colorBuffers[i]->getObjectType() == IFramebufferObject::FBO_ATTACHMENT_TEXTURE)
            glFramebufferTexture2D(GL_FRAMEBUFFER, colorBuffers[i]->getAttachment() + i, colorBuffers[i]->getAttachmentTarget(), colorBuffers[i]->getObjectId(), 0);
        else
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, colorBuffers[i]->getAttachment() + i, colorBuffers[i]->getAttachmentTarget(), colorBuffers[i]->getObjectId());
        m_activeBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    if (depthBuffer->getObjectType() == IFramebufferObject::FBO_ATTACHMENT_TEXTURE)
        glFramebufferTexture2D(GL_FRAMEBUFFER, depthBuffer->getAttachment(), depthBuffer->getAttachmentTarget(), depthBuffer->getObjectId(), 0);
    else
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthBuffer->getAttachment(), depthBuffer->getAttachmentTarget(), depthBuffer->getObjectId());

    // reset draw/read buffer states - this is sometime causing trouble
    //glDrawBuffer(GL_BACK);
    //glReadBuffer(GL_BACK);

    uint32_t status = GL_FRAMEBUFFER_COMPLETE;
    if (checkCompleteness)
        status = (uint32_t)glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
        printf("glCheckFramebufferStatus error: %d\n", status);

    // unbind is necessary to avoid interferring with other render targets
    unbind();

    return status;
}

//-------------------------------------------------------------------------------------------------

void GLFramebufferObject::bind()
{
    if (m_activeBuffers.empty())
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
    glDrawBuffers((GLsizei)m_activeBuffers.size(), (GLenum*)m_activeBuffers.data());
}

//-------------------------------------------------------------------------------------------------

void GLFramebufferObject::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------------------------------------------------------

void GLFramebufferObject::blitToBuffer(uint32_t targetBufferID, uint32_t targetAttachment)
{
#ifdef _DEBUG
    PRINT_GL_ERROR("GLFramebufferObject::blitToBuffer error at start: %d\n")
#endif // _DEBUG

    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
    PRINT_GL_ERROR("GLFramebufferObject::blitToBuffer error glBindFramebuffer 1: %d\n")

    glReadBuffer(GL_COLOR_ATTACHMENT0);
    PRINT_GL_ERROR("GLFramebufferObject::blitToBuffer error glReadBuffer: %d\n")

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetBufferID);
    PRINT_GL_ERROR("GLFramebufferObject::blitToBuffer error glBindFramebuffer 2: %d\n")
    
    //glDrawBuffer(targetAttachment);

    glBlitFramebuffer(0, 0, 2048, 2048, 0, 0, 2048, 2048, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    PRINT_GL_ERROR("GLFramebufferObject::blitToBuffer error glBlitFramebuffer: %d\n")
}

//-------------------------------------------------------------------------------------------------
