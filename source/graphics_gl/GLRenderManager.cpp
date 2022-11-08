//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLRenderManager.h>
//-------------------------------------------------------------------------------------------------
#include <chrono>
#include <graphics/RenderCommand.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/framegraph/Framegraph.h>
#include <graphics/geometry/ScreenQuadGeometry.h>
#include <graphics_gl/GLGeometryBufferCreator.h>
#include <graphics_gl/GLRenderCommandGenerator.h>
#include <graphics_gl/GLRenderable.h>
#include <graphics_gl/GLShaderCreator.h>
#include <graphics_gl/GeometryPass.h>
#include <graphics_gl/GLFramebufferObject.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

using namespace dke::graphics;

//-------------------------------------------------------------------------------------------------

GLRenderManager::GLRenderManager()
    : RenderManager(new DefaultRenderableKeyGenerator())
    , m_lastUpdateTimestamp(0)
{
    //    m_apTextureManager.reset(new GLTextureManager());
    setGeometryBufferCreator(new GLGeometryBufferCreator);
    setShaderCreator(new GLShaderCreator);
    //m_fbo = std::unique_ptr<IFramebufferObject>(createFBO());
}

//-------------------------------------------------------------------------------------------------

GLRenderManager::~GLRenderManager()
{
    reset();
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::initialize()
{
    const GLubyte* versionStr = glGetString(GL_VERSION);

    // get capabilities of this GL implementation.
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &m_caps.maxUniformBufferSize);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_caps.maxVertexUniformVectors);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &m_caps.maxElementIndices);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &m_caps.maxElementVertices);
    glGetInteger64v(GL_MAX_ELEMENT_INDEX, &m_caps.maxElementIndex);
    

    m_fbo.reset(createFBO());
    onInitialize();
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::cacheGeometries()
{
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::reset()
{
    RenderManager::reset();
}

//-------------------------------------------------------------------------------------------------

graphics::IFramebufferObject* GLRenderManager::getFBO()
{
    return m_fbo.get();
}

//-------------------------------------------------------------------------------------------------

IFramebufferObject* GLRenderManager::createFBO()
{
    GLFramebufferObject* fbo = new GLFramebufferObject();
    fbo->create();
    return fbo;
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::onUpdate()
{
    // TODO: should promote the implementation to base class

    for (const auto& r : renderables()) {
        if (r->isUpdated(lastUpdateTimestamp())) {
            //CMarkupRenderable* buttonRenderable = dynamic_cast<CMarkupRenderable*>(r.get());
            //if (!buttonRenderable)
            //continue;
            Renderable* renderable = r.get();

            // check if geometry is updated
            std::vector<std::unique_ptr<Geometry>> geometries;
            renderable->onGeometryUpdate(geometries);

            bool hasUnMatchHandle = false;

            // check geometry size against its supposed handle
            for (int i = 0; i < geometries.size(); i++) {
                bool matches = true;
                if (renderable->numGeometryHandles() != geometries.size())
                    matches = false;
                else {
                    const Geometry::GeometryBufferHandle& h = renderable->getGeometryHandle(i);
                    if (h.dataCount != geometries[i]->getAttributeDataSize(0) * geometries[i]->getAttributeComponentSize(0)) {
                        matches = false;
                    }
                    // if matches set the buffer handle to geometry so that it
                    // gets updated, otherwise the geometry will be cached with
                    // invalid handle
                    if (matches)
                        geometries[i]->setBufferHandle(h);
                }

                geometries[i]->setDirty(true);

                // TODO: fix this cache twice problem.
                // first cache add geometry to buffer but not caching it. This is for speed when adding
                // a lot of geometries from drawing.
                m_geometryManager.cacheGeometry(geometries[i].get());
                // second cache with geometry buffer handle actually cache the geometry into buffers.
                m_geometryManager.cacheGeometry(geometries[i].get());

                renderable->setGeometryHandle(i, geometries[i]->bufferHandle());

                if (!matches)
                    hasUnMatchHandle = true;
            }

            // check if render command needs to be updated
            if (renderable->getNumRenderCommands() != geometries.size()) {
                // remove old render commands
                auto it = std::remove_if(m_renderCommands.begin(), m_renderCommands.end(),
                    [&](graphics::RenderCommand* rc) {
                        for (int i = 0; i < renderable->getNumRenderCommands(); i++) {
                            if (rc->id() == renderable->getRenderCommands()[i]->id())
                                return true;
                        }
                        return false;
                    });
                if (it != m_renderCommands.end())
                    m_renderCommands.erase(it, m_renderCommands.end());

                // generate new ones
                GLRenderCommandGenerator generator(this);
                std::vector<RenderCommand*> commands = generator.generate(renderable, geometries);
                renderable->setRenderCommands(commands);
                int currentIndex = 0;
                for (RenderCommand* pCommand : commands) {
                    pCommand->setRenderable(renderable);
                    pCommand->setGeometry(nullptr);
                    pCommand->setGeometryHandle(renderable->getGeometryHandle(currentIndex++));
                }

                // also place them in the "global" render commands list
                std::vector<RenderCommand*>& renderCommands = m_renderCommands;
                renderCommands.insert(renderCommands.end(), commands.begin(), commands.end());
            } else if (hasUnMatchHandle) {
                for (int i = 0; i < renderable->getNumRenderCommands(); i++) {
                    renderable->getRenderCommands()[i]->setGeometryHandle(renderable->getGeometryHandle(i));
                }
            }
        }
    }

    updateLastTimestamp();
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::onRender()
{
}

//-------------------------------------------------------------------------------------------------

uint64_t GLRenderManager::lastFrameTime()
{
    return m_lastUpdateTimestamp - m_lastLastUpdateTimestamp;
}

//-------------------------------------------------------------------------------------------------

uint64_t GLRenderManager::lastUpdateTimestamp()
{
    return m_lastUpdateTimestamp;
}

//-------------------------------------------------------------------------------------------------

void GLRenderManager::updateLastTimestamp()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    m_lastLastUpdateTimestamp = m_lastUpdateTimestamp;
    m_lastUpdateTimestamp = ms.count();
}

//-------------------------------------------------------------------------------------------------
}
}
//-------------------------------------------------------------------------------------------------
