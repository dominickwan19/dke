//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLRenderCommandGenerator.h>
//-------------------------------------------------------------------------------------------------
#include <chrono>
#include <graphics/Buffer.h>
#include <graphics/RenderManager.h>
#include <graphics/Shader.h>
#include <graphics/ShaderManager.h>
#include <graphics_gl/GLRenderCommand.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics/UniformBuffer.h>
#include <GL/glew.h>
//-------------------------------------------------------------------------------------------------
#include <set>
#include <unordered_set>
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

//// This needs to be defined for either the GL platform of the GLES platform.
//extern RenderCommand* createGLCommand(int32_t type);

//-------------------------------------------------------------------------------------------------

GLRenderCommandGenerator::GLRenderCommandGenerator(RenderManager* renderManager)
    : m_renderManager(renderManager)
{
}

//-------------------------------------------------------------------------------------------------

std::vector<RenderCommand*> GLRenderCommandGenerator::generate(Renderable* renderable, std::vector<std::unique_ptr<Geometry>>& geometries) const
{
    std::vector<RenderCommand*> rndCmds;
    if (!renderable)
        return rndCmds;

    uint64_t key = renderable->getKey();
    for (auto& geometry : geometries) {
        if (geometry->type() == GL_LINES) {
            if (RenderCommand* pRenderCommand = generateLines(key, renderable, geometry.get(), nullptr, 0, 0))
                rndCmds.push_back(pRenderCommand);
        } else if (geometry->type() == GL_LINE_STRIP) {
            if (RenderCommand* pRenderCommand = generateLineStrip(key, renderable, geometry.get(), nullptr, 0, 0))
                rndCmds.push_back(pRenderCommand);
        } else if (geometry->type() == GL_TRIANGLES) {
            if (RenderCommand* pRenderCommand = generateMesh(key, renderable, geometry.get(), nullptr, 0, 0))
                rndCmds.push_back(pRenderCommand);
        }
    }

    return rndCmds;
}

//-------------------------------------------------------------------------------------------------

RenderCommand* GLRenderCommandGenerator::generateLines(uint64_t key, Renderable* renderable, Geometry* geometry, UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const
{
    const Geometry::GeometryBufferHandle& handle = geometry->bufferHandle();

    GeometryAttributeBase* pAttribute = geometry->getAttribute(0);

    RenderCommand* rc = new GLRenderCommand();
    DrawArraysData* drawArraysData = new DrawArraysData();
    drawArraysData->geometryType = GL_LINES;
    drawArraysData->first = handle.startPosition / pAttribute->getTypeSizeInBytes() / pAttribute->getNumComponents();
    drawArraysData->dataCount = handle.dataCount / pAttribute->getNumComponents();
    rc->setDrawData(drawArraysData);
    rc->setGeometryBufferId(handle.geometryBufferId);
    rc->setGeometryBuffer(m_renderManager->getGeometryBuffer(handle.geometryBufferId));
    rc->setKey(key);
    rc->setRenderable(renderable);
    rc->setGeometry(geometry);
    rc->setShaderId(renderable->material());
    rc->setTextureId(geometry->textureId());

    GLRenderCommand* glrc = dynamic_cast<GLRenderCommand*>(rc);
    glrc->setInstanceData(offset, size);
    glrc->setUniformBuffer(uniformBuffer);

    if (renderable)
        renderable->addRenderCommand(rc);

    return rc;
}

//-------------------------------------------------------------------------------------------------

RenderCommand* GLRenderCommandGenerator::generateLineStrip(uint64_t key, Renderable* renderable, Geometry* geometry, UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const
{
    const Geometry::GeometryBufferHandle& handle = geometry->bufferHandle();

    GeometryAttributeBase* pAttribute = geometry->getAttribute(0);

    int32_t byteSize = pAttribute->getTypeSizeInBytes();
    int32_t numComponents = pAttribute->getNumComponents();

    RenderCommand* rc = rc = new GLRenderCommand(); 
    DrawArraysData* drawArraysData = new DrawArraysData();
    drawArraysData->geometryType = GL_LINE_STRIP;
    drawArraysData->first = handle.startPosition / byteSize / numComponents;
    drawArraysData->dataCount = handle.dataCount / numComponents;
    rc->setDrawData(drawArraysData);
    rc->setGeometryBufferId(handle.geometryBufferId);
    // printf("geometry buffer iD: %d\n", rc->geometryBufferId);
    rc->setGeometryBuffer(m_renderManager->getGeometryBuffer(handle.geometryBufferId));
    rc->setKey(key);
    rc->setRenderable(renderable);
    rc->setGeometry(geometry);
    rc->setShaderId(renderable->material());
    rc->setTextureId(geometry->textureId());

    GLRenderCommand* glrc = dynamic_cast<GLRenderCommand*>(rc);
    glrc->setInstanceData(offset, size);
    glrc->setUniformBuffer(uniformBuffer);

    if (renderable)
        renderable->addRenderCommand(rc);

    return rc;
}

//-------------------------------------------------------------------------------------------------

RenderCommand* GLRenderCommandGenerator::generateMesh(uint64_t key, Renderable* renderable, Geometry* geometry, UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const
{
    const Geometry::GeometryBufferHandle& handle = geometry->bufferHandle();

    GeometryAttributeBase* pAttribute = geometry->getAttribute(0);

    int32_t byteSize = pAttribute->getTypeSizeInBytes();
    int32_t numComponents = pAttribute->getNumComponents();

    RenderCommand* rc = rc = new GLRenderCommand(); 
    DrawData* data = nullptr;
    if (geometry->getNumIndices() == 0) {
        DrawArraysData* drawArraysData = new DrawArraysData();
        drawArraysData->geometryType = GL_TRIANGLES;
        drawArraysData->first = handle.startPosition;
        drawArraysData->dataCount = handle.dataCount;
        data = drawArraysData;
    }
    else {
        DrawElementsData* drawElementsData = new DrawElementsData();
        drawElementsData->geometryType = GL_TRIANGLES;
        drawElementsData->dataCount = handle.indexCount;
        drawElementsData->indexStart = handle.indexStart;
        drawElementsData->baseVertex = handle.startPosition / byteSize / numComponents;
        data = drawElementsData;
    }
    rc->setDrawData(data);
    rc->setGeometryBufferId(handle.geometryBufferId);
    rc->setGeometryBuffer(m_renderManager->getGeometryBuffer(handle.geometryBufferId));
    rc->setKey(key);
    rc->setRenderable(renderable);
    rc->setGeometry(geometry);
    rc->setShaderId(renderable->material());
    rc->setTextureId(geometry->textureId());
    rc->setActive(true);

    if (renderable)
        renderable->addRenderCommand(rc);

    return rc;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
