//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderManager_h
#define DKE_Graphics_RenderManager_h
//-------------------------------------------------------------------------------------------------
#include "graphics/Buffer.h"
#include "graphics/CRenderableFactory.h"
#include "graphics/GeometryManager.h"
#include "graphics/MaterialManager.h"
#include "graphics/RenderOptions.h"
#include "graphics/RenderQueue.h"
#include "graphics/ShaderManager.h"
#include "graphics/framegraph/Framegraph.h"
#include <graphics/Colour.h>
#include <set>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Forward Declarations.
class Camera;
class IFramebufferObject;
class RenderableKeyGenerator;
class RenderCommand;
class RenderPassBase;
class Shader;
class TextureManager;

//-------------------------------------------------------------------------------------------------

/*!
 * \brief   RenderManager manages everything for rendering.
 */
class RenderManager {
public:
    RenderManager(RenderableKeyGenerator* keyGen);
    virtual ~RenderManager();

    struct RenderCapabilities {
        int32_t maxUniformBufferSize;
        int32_t maxVertexUniformVectors;
        int32_t maxElementIndices;
        int32_t maxElementVertices;
        int64_t maxElementIndex;
    };

    enum class TBackgroundStyle {
        Standard = 0,
        Paper,
        Gradient
    };

    virtual void initialize() {}

    void setActiveCamera(const Camera* camera);
    const Camera* activeCamera();

    template <typename RenderPassData, typename... RenderPassArguments>
    RenderPass<RenderPassData>* createRenderPass(RenderPassArguments&&... args)
    {
        return m_apFramegraph->createRenderPass<RenderPassData>(args...);
    }
    RenderPassBase* addRenderPass(RenderPassBase* renderPass);

    //void setBackgroundStyle(TBackgroundStyle backgroundStyle, const exmaths::CPntArryPtr& apPaperPoints);
    //TBackgroundStyle backgroundStyle() const { return m_backgroundStyle; }
    //const exmaths::CPntArryPtr& paperPoints() const { return m_apPaperPoints; }

    void setGeometryBufferCreator(GeometryBufferCreator* creator);
    void setShaderCreator(ShaderCreator* creator);

    // Permanent means will not be cleared during reset().
    void addRenderable(Renderable* renderable, bool permanent = false);
    void removeRenderable(Renderable* renderable);
    Renderable* findRenderable(int32_t renderableId) const;

    TextureManager* getTextureManager() const { return m_apTextureManager.get(); }
    const ShaderManager* getShaderManager() const { return &m_shaderManager; }

    Shader* findShader(int32_t shaderType) const;

    Framegraph& framegraph() const;
    void updateFrame();
    void renderFrame(const Camera* camera);
    void compileFramegraph();
    virtual void cacheGeometries() {}
    void cacheGeometriesAndBuffers();
    void cacheGeometry(Renderable* r);
    void cacheGeometry(Geometry* g);

    virtual void reset();

    template <class T>
    void registerRenderableType()
    {
        assert((std::is_base_of<Renderable, T>::value)); // Template class A must derive from CAction

        const std::type_info& tinfo = typeid(T);
        if (m_factoryMap.find(tinfo.hash_code()) == m_factoryMap.end()) {
            IRenderableFactoryPtr factory = std::static_pointer_cast<IRenderableFactory>(std::make_shared<CRenderableFactory<T>>());
            m_factoryMap.insert(std::make_pair(tinfo.hash_code(), factory));
        }
    }

    // Create dynamic renderables that are not loaded from drawing.
    // (Static renderables from drawing are loaded with separate flow,
    // but still using functions in this manager. Should look
    // to unify.)
    template <class R, class... Args>
    R* createRenderable(Args&&... args)
    {
        assert((std::is_base_of<Renderable, R>::value)); // Template class A must derive from Renderable

        const std::type_info& tinfo = typeid(R);
        auto iter = m_factoryMap.find(tinfo.hash_code());
        if (iter == m_factoryMap.end())
            return nullptr;

        const IRenderableFactoryPtr& factory = iter->second;

        Renderable* rBase = factory->create();
        R* r = dynamic_cast<R*>(rBase);
        if (!r)
            return nullptr;

        r->initialize(std::forward<Args>(args)...);

        // TODO: should consider removing this from createRenderable.
        r->createGeometry();

        addRenderable(r);

        return r;
    }

    std::vector<std::unique_ptr<Renderable>>& renderables() { return m_renderables; }

    RenderQueue* getRenderQueue() { return &m_renderQueue; }
    GeometryBuffer* getGeometryBuffer(int id) const { return m_geometryManager.getBuffer(id); }

    virtual IFramebufferObject* createFBO();
    virtual void captureScreen(const std::string& outputFileName);

    // most likely to be moved to geometry manager
    virtual void updateGeometry(Geometry* geometry);
    virtual void updateAllGeometries();

    RenderOptions* renderOptions() { return &m_renderOptions; }
    const RenderOptions* renderOptions() const { return &m_renderOptions; }
    const RenderCapabilities& renderCapabilities() const { return m_caps; }

    virtual Buffer* createDataBuffer(uint32_t target, uint32_t size, uint32_t bindingPoint);

    int32_t getGeometryBufferSize();

protected:
    virtual void onInitialize();
    virtual void onUpdate();
    virtual void onRender();

protected:
    std::unique_ptr<TextureManager> m_apTextureManager;

    MaterialManager m_materialManager;
    GeometryManager m_geometryManager;
    ShaderManager m_shaderManager;
    std::unique_ptr<RenderableKeyGenerator> m_keyGenerator;

    RenderQueue m_renderQueue;
    std::unique_ptr<Framegraph> m_apFramegraph;

    std::vector<std::unique_ptr<Renderable>> m_renderables;
    std::vector<std::unique_ptr<Buffer>> m_dataBuffers; // uniform buffers
    std::vector<graphics::RenderCommand*> m_renderCommands;

    std::unordered_map<size_t, IRenderableFactoryPtr> m_factoryMap;

    RenderOptions m_renderOptions;
    RenderCapabilities m_caps;

    TBackgroundStyle m_backgroundStyle;
    const Camera* m_camera;
};

typedef std::shared_ptr<RenderManager> RenderManagerPtr;

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderManager_h
