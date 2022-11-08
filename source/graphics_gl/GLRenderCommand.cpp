//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLRenderCommand.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
#include <graphics/ITextureObject.h>
#include <graphics/Shader.h>
#include <GL/glew.h>
//-------------------------------------------------------------------------------------------------

// for casting int to void* as the buffer offset
#pragma warning(disable : 4312)

using namespace dke::graphics;

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

//// better use a factory class.
//RenderCommand* createGLCommand(int32_t type)
//{
//    switch (type) {
//    case RENDER_COMMAND_STENCIL:
//        return new GLStencilCommand();
//    case RENDER_COMMAND_BASIC:
//    default:
//        return new GLRenderCommand();
//    }
//}

//-------------------------------------------------------------------------------------------------

GLRenderCommand::GLRenderCommand()
    : RenderCommand()
    , m_instanceDataOffset()
    , m_instanceDataSize()
    , m_uniformBuffer(nullptr)
{
}

//-------------------------------------------------------------------------------------------------

void GLRenderCommand::setUniformBuffer(UniformBuffer* ub)
{
    m_uniformBuffer = ub;
}

//-------------------------------------------------------------------------------------------------

void GLRenderCommand::setInstanceData(int32_t offset, int32_t size)
{
    m_instanceDataOffset = offset;
    m_instanceDataSize = size;
}

//-------------------------------------------------------------------------------------------------

void GLRenderCommand::execute(const RenderManager* renderManager, Shader* shader, RenderOptions* renderOptions)
{
    if (geometry()) {
        if (geometry()->bufferHandle().geometryBufferId < 0)
            return;
    } else if (geometryBufferId() < 0) {
        return;
    }

    if (m_renderable)
        m_renderable->onBindParameters(this, shader);

    //if (isHighlighted()) {
    //    shader->setUniform("highlightColour", highlightColour().dev_red(), highlightColour().dev_green(), highlightColour().dev_blue(), highlightColour().dev_alpha());
    //} else {
    //    shader->setUniform("highlightColour", 0.0f, 0.0f, 0.0f, 0.0f);
    //}

    switch (drawData()->typeId()) {
    case RC_DRAW_ARRAYS: {
        const DrawArraysData* dd = (const DrawArraysData*)drawData();

        if (m_uniformBuffer)
            m_uniformBuffer->bindRange(m_instanceDataOffset, m_instanceDataSize);

        ITextureObject* texObj = nullptr;

        // next step is to get rid of this texture bind and set uniform. The parameter
        // if required can be packed into uniform buffer. Texture handling code can
        // be done in a GLTexturedRenderCommand class.
        // get rid of isAlphaTexture flag as well when doing above.
        if (texObj) {
            glActiveTexture(GL_TEXTURE0);
            texObj->bind();
            shader->setUniform("useTex", 1.0f);
            if (texObj->getFormat() == GL_ALPHA)
                shader->setUniform("isAlphaTexture", 1.0f);
            else
                shader->setUniform("isAlphaTexture", 0.0f);
            //glEnable(GL_BLEND);
        } else {
            shader->setUniform("useTex", 0.0f);
        }

        if (m_geometryBuffer) {
            m_geometryBuffer->bind();
        } else {
            if (geometry())
                setGeometryBufferId(geometry()->bufferHandle().geometryBufferId);

            if (GeometryBuffer* buff = renderManager->getGeometryBuffer(geometryBufferId()))
                buff->bind();
        }
        if (!geometry()) {
            if (!m_geometryBuffer)
                return;
            auto gfd = m_geometryBuffer->getFormatDescription();
            glDrawArrays(dd->geometryType, geometryHandle().startPosition / (gfd.typeSizes[0] * gfd.componentSizes[0]), geometryHandle().dataCount / gfd.componentSizes[0]);
        } else {
            GeometryAttributeBase* pAttribute = geometry()->getAttribute(0);
            glDrawArrays(dd->geometryType, geometry()->bufferHandle().startPosition / pAttribute->getTypeSizeInBytes() / pAttribute->getNumComponents(), geometry()->getDataCount() / pAttribute->getNumComponents());
        }

        if (texObj) {
            texObj->unbind();
            //glDisable(GL_BLEND);
        }
    } break;
    case RC_DRAW_ELEMENTS: {
        const DrawElementsData* dd = (const DrawElementsData*)drawData();

        if (m_uniformBuffer)
            m_uniformBuffer->bindRange(m_instanceDataOffset, m_instanceDataSize);

        ITextureObject* texObj = nullptr;
        if (textureId())
            texObj = renderManager->getTextureManager()->findTexture(textureId());

        // next step is to get rid of this texture bind and set uniform. The parameter
        // if required can be packed into uniform buffer. Texture handling code can
        // be done in a GLTexturedRenderCommand class.
        // get rid of isAlphaTexture flag as well when doing above.
        if (texObj) {
            glActiveTexture(GL_TEXTURE0);
            texObj->bind();
            shader->setUniform("useTex", 1.0f);
            if (texObj->getFormat() == GL_ALPHA)
                shader->setUniform("isAlphaTexture", 1.0f);
            else
                shader->setUniform("isAlphaTexture", 0.0f);
            //glEnable(GL_BLEND);
        } else {
            shader->setUniform("useTex", 0.0f);
        }

        if (m_geometryBuffer) {
            m_geometryBuffer->bind();
        } else {
            if (geometry())
                setGeometryBufferId(geometry()->bufferHandle().geometryBufferId);

            GeometryBuffer* buff = renderManager->getGeometryBuffer(geometryBufferId());
            if (!buff)
                return;
            buff->bind();
        }
        // &dd->indexStart doesn't seem to be correct.
        //glDrawElementsBaseVertex(dd->geometryType, dd->dataCount, GL_UNSIGNED_INT, &dd->indexStart, dd->baseVertex);
        if (geometry())
            glDrawElements(dd->geometryType, geometry()->bufferHandle().indexCount, GL_UNSIGNED_INT, (const void*)(geometry()->bufferHandle().indexStart * sizeof(GLuint)));
        else
            glDrawElements(dd->geometryType, geometryHandle().indexCount, GL_UNSIGNED_INT, (const void*)(geometryHandle().indexStart * sizeof(GLuint)));

        if (texObj) {
            texObj->unbind();
            //glDisable(GL_BLEND);
        }
    } break;
    case RC_MULTI_DRAW_ELEMENTS: {
        const MultiDrawElementsData* dd = (const MultiDrawElementsData*)drawData();
        if (m_geometryBuffer) {
            m_geometryBuffer->bind();
        } else {
            GeometryBuffer* buff = renderManager->getGeometryBuffer(geometryBufferId());
            buff->bind();
        }
        //glMultiDrawElementsBaseVertex(dd->geometryType, dd->indices, GL_UNSIGNED_INT, (const void* const*)dd->indexStarts, dd->dataCount, dd->baseVertices);
    } break;
    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
