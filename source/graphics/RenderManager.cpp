//-------------------------------------------------------------------------------------------------
#include <graphics/RenderManager.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryBuffer.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/Shader.h>
#include <graphics/framegraph/RenderPass.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

RenderManager::RenderManager(RenderableKeyGenerator* keyGen)
    : m_keyGenerator(keyGen)
    , m_geometryManager(this)
    , m_backgroundStyle(TBackgroundStyle::Standard)
    , m_apFramegraph(new Framegraph())
{
    m_caps.maxUniformBufferSize = 0;
    m_caps.maxVertexUniformVectors = 0;
    m_caps.maxElementIndices = 0;
    m_caps.maxElementVertices = 0;
    m_caps.maxElementIndex = 0;
}

//-------------------------------------------------------------------------------------------------

RenderManager::~RenderManager()
{
}

//-------------------------------------------------------------------------------------------------

void RenderManager::reset()
{
    m_geometryManager.deleteBuffers();
}

//-------------------------------------------------------------------------------------------------

RenderPassBase* RenderManager::addRenderPass(RenderPassBase* renderPass)
{
    return m_apFramegraph->addRenderPass(renderPass);
}

//-------------------------------------------------------------------------------------------------

Shader* RenderManager::findShader(int shaderType) const
{
    return m_shaderManager.find(shaderType);
}

//-------------------------------------------------------------------------------------------------

void RenderManager::setGeometryBufferCreator(GeometryBufferCreator* creator)
{
    m_geometryManager.setGeometryBufferCreator(creator);
}

//-------------------------------------------------------------------------------------------------

void RenderManager::setShaderCreator(ShaderCreator* creator)
{
    m_shaderManager.setShaderCreator(creator);
}

//-------------------------------------------------------------------------------------------------

void RenderManager::addRenderable(Renderable* renderable, bool permanent)
{
    if (!renderable->material())
        printf("RenderManager::addRenderable material==0 id=%d\n", renderable->id());

    //assert(renderable->keyStruct.material != 0);

    //renderable->setKey(m_keyGenerator->generateKey(&renderable->keyStruct()));
    //renderable->getKey(); // update the key. Wonder if this is required.

    // with renderable type, find appropriate geometry store (bucket?) to handle the renderable geometry
    // should find collection for geometry but not renderable

    /*
    for (Geometry* g : renderable->geometries()) {
        m_geometryManager.cacheGeometry(g);
    }
    */

    m_renderables.emplace_back(renderable);
}

//-------------------------------------------------------------------------------------------------

void RenderManager::removeRenderable(Renderable* renderable)
{
    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        if (it->get() == renderable) {
            m_renderables.erase(it);
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------

Renderable* RenderManager::findRenderable(int32_t renderableId) const
{
    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        if (it->get()->globalId() == renderableId)
            return it->get();
    }
    return nullptr;
}

//-------------------------------------------------------------------------------------------------

Framegraph& RenderManager::framegraph() const
{
    return *m_apFramegraph.get();
}

//-------------------------------------------------------------------------------------------------

void RenderManager::updateFrame()
{
    // for all renderables
    // 1. calculate depth (not done)
    // 2. calculate visibility (not done)
    // 3. update key for each renderables
    // 4. update draw commands from renderables for this frame
    //    *each renderable could be given a different scheme for generating draw commands,
    //     e.g. LODRenderable are using distance to determine which geometry will be used

    //for (Renderable* r : m_renderables) {
    //    r->setKey(m_keyGenerator->generateKey(&r->keyStruct)); // 3 of above
    //    //r->updateRenderCommands(m_keyGenerator); // 4 of above
    //}
    onUpdate(); // do things mentioned above, and geometry update

    //// submit the renderable to render queue - there would be some scheme apply
    //// TODO: here we just do ad-hoc submit - to be sorted out
    //for (Renderable* r : m_renderables) {
    //    m_renderQueue.add(r->getRenderCommands(), r->getNumRenderCommands());
    //}

    m_renderQueue.clear();
    m_renderQueue.add(m_renderCommands.data(), m_renderCommands.size());
    m_renderQueue.sort();
}

//-------------------------------------------------------------------------------------------------

void RenderManager::renderFrame(const Camera* camera)
{
    onRender();

    m_apFramegraph->execute(camera, renderOptions());
}

//-------------------------------------------------------------------------------------------------

void RenderManager::onInitialize()
{
}

//-------------------------------------------------------------------------------------------------

 void RenderManager::onUpdate()
{

}

 //-------------------------------------------------------------------------------------------------

 void RenderManager::onRender()
 {

 }

 //-------------------------------------------------------------------------------------------------

void RenderManager::compileFramegraph()
{
    m_apFramegraph->compile();
}

//-------------------------------------------------------------------------------------------------

void RenderManager::cacheGeometriesAndBuffers()
{
    // (a) Cache all geometries for all renderables within the render manager, 
    // and (b) remove geometry data from host memory if applicable. 
    // The process needs to be refined:
    // - This function is called explicitly. It should be handled within 
    //   the render pipeline.
    // - Process (b) should be processed separately but will within
    //   the render pipeline.
    // - Some other classes should decide what data to be cleared for (b).
    for (const auto& r : m_renderables) {
        cacheGeometry(r.get()); // (a)
        r.get()->clearGeometryData(); // (b)
    }
    
    m_geometryManager.cacheGeometry(); // (a), actually caching buffers as well
    m_geometryManager.clearHostStaticData(); // (b)
}

//-------------------------------------------------------------------------------------------------

void RenderManager::cacheGeometry(Renderable* r)
{
    for (auto g : r->geometries())
        m_geometryManager.cacheGeometry(g);
}

//-------------------------------------------------------------------------------------------------

void RenderManager::cacheGeometry(Geometry* g)
{
    m_geometryManager.cacheGeometry(g);
}

//-------------------------------------------------------------------------------------------------

IFramebufferObject* RenderManager::createFBO()
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------

void RenderManager::captureScreen(const std::string& outputFilename)
{
    printf("RenderManager::captureScreen outputFilename=%s\n", outputFilename.c_str());
}

//-------------------------------------------------------------------------------------------------

void RenderManager::updateGeometry(Geometry* geometry)
{
}

//-------------------------------------------------------------------------------------------------

void RenderManager::updateAllGeometries()
{
}

//-------------------------------------------------------------------------------------------------

Buffer* RenderManager::createDataBuffer(uint32_t target, uint32_t size, uint32_t bindingPoint)
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------

int32_t RenderManager::getGeometryBufferSize()
{
    return m_geometryManager.getGeometryBufferSize();
}

//-------------------------------------------------------------------------------------------------

void RenderManager::setActiveCamera(const Camera* camera)
{
    m_camera = camera;
}

//-------------------------------------------------------------------------------------------------

const Camera* RenderManager::activeCamera()
{
    return m_camera;
}

//-------------------------------------------------------------------------------------------------

void RenderManager::onResize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
