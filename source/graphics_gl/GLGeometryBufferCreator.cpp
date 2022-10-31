//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLGeometryBufferCreator.h>
//-------------------------------------------------------------------------------------------------
#include <graphics_gl/ScreenQuadGeometryBuffer.h>
#include <GL/glew.h>
//#include <ex_rendering_platform.h>
//#include <graphics/RenderManager.h>
//#include <rendering/CTextGeometryBuffer.h>
//#include <rendering/WDELineGeometryBuffer.h>
//#include <rendering/WDELineStripGeometryBuffer.h>
//#include <rendering/WDEMeshGeometryBuffer.h>
//#include <rendering/WDEScreenQuadGeometryBuffer.h>
//#include <rendering/ex_renderingDefines.h>
//#include <rendering/geometry/CLineWidthGeometryBuffer.h>
//-------------------------------------------------------------------------------------------------

using namespace dke::graphics;

namespace dke {
namespace graphics {
// This needs to be defined for either the GL platform of the GLES platform.
extern dke::graphics::Buffer* createGLBuffer(GLuint bufferTarget, size_t bufferSize);
}
}

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

GLGeometryBufferCreator::GLGeometryBufferCreator()
{
}

GeometryBuffer* GLGeometryBufferCreator::createBuffer(uint32_t geometryType, uint32_t flags, const GeometryFormatDescription& gfd, size_t bufferSize)
{
    //if (geometryType == GL_LINES) {
    //    WDELineGeometryBuffer* lineBuffer = new WDELineGeometryBuffer(gfd, flags, bufferSize);
    //    lineBuffer->setCreateBufferFunction(createGLBuffer);
    //    lineBuffer->create();
    //    return lineBuffer;
    //}

    //if (geometryType == GL_LINE_STRIP) {
    //    WDELineStripGeometryBuffer* lineStripBuffer = new WDELineStripGeometryBuffer(gfd, flags, bufferSize);
    //    lineStripBuffer->setCreateBufferFunction(createGLBuffer);
    //    lineStripBuffer->create();
    //    return lineStripBuffer;
    //}

    //if (geometryType == GL_TRIANGLES && gfd.numAttributes > 1) {
    //    WDEMeshGeometryBuffer* meshBuffer = new WDEMeshGeometryBuffer(gfd, flags, bufferSize);
    //    meshBuffer->setCreateBufferFunction(createGLBuffer);
    //    meshBuffer->create();
    //    meshBuffer->cache();
    //    return meshBuffer;
    //}

    if (geometryType == GL_TRIANGLES && gfd.numAttributes == 1) {
        // perhaps should default a buffer implementation to store some standard stuff
        ScreenQuadGeometryBuffer* screenQuadBuffer = new ScreenQuadGeometryBuffer(gfd, flags, 1024);
        screenQuadBuffer->setCreateBufferFunction(createGLBuffer);
        // TODO: simplify this - should call create and cache together in constructor
        screenQuadBuffer->create();
        screenQuadBuffer->cache();
        return screenQuadBuffer;
    }

    //if (geometryType == kGeometryText) {
    //    CTextGeometryBuffer* textGeometryBuffer = new CTextGeometryBuffer(gfd, flags, bufferSize);
    //    textGeometryBuffer->setCreateBufferFunction(createGLBuffer);
    //    textGeometryBuffer->create();
    //    return textGeometryBuffer;
    //}

    //if (geometryType == kGeometryLineWidth) {
    //    CLineWidthGeometryBuffer* lineWidthGeometryBuffer = new CLineWidthGeometryBuffer(gfd, flags, bufferSize, m_renderManager);
    //    lineWidthGeometryBuffer->setCreateBufferFunction(createGLBuffer);
    //    lineWidthGeometryBuffer->create();

    //    return lineWidthGeometryBuffer;
    //}

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace exrendering
//-------------------------------------------------------------------------------------------------
