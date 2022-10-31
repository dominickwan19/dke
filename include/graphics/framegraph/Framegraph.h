//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Framegraph_h
#define DKE_Graphics_Framegraph_h
//-------------------------------------------------------------------------------------------------
#include "RenderPass.h"
#include "graphics/RenderOptions.h"
#include "Resource.h"
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------
class Camera;

class Framegraph {
public:
    Framegraph() = default;
    ~Framegraph() = default;

    template <typename RenderPassData, typename... RenderPassArguments>
    RenderPass<RenderPassData>* createRenderPass(RenderPassArguments&&... args);

    RenderPassBase* addRenderPass(RenderPassBase* renderPass);

    void compile();

    void execute(const Camera* camera, const RenderOptions* renderOptions);

    void clear() {}

    template <typename ContentType, typename ContentDescriptionType>
    void createResourceHandle(const std::string& name, int32_t resourceType, ContentDescriptionType& description);

    template <typename ContentType>
    ContentType* getResourceHandle(const std::string& name);

    const std::vector<std::unique_ptr<RenderPassBase>>& renderPasses() { return m_renderPasses; }

    //private:
    //void buildTimeline();
    //Timeline m_timeline;

private:
    friend RenderPassBuilder;

    std::vector<std::unique_ptr<RenderPassBase>> m_renderPasses;
    std::vector<std::vector<ResourceHandleBase*>> m_resourceList;

    std::unordered_map<std::string, std::unique_ptr<ResourceHandleBase>> m_resourceHandleMap;
};

//-------------------------------------------------------------------------------------------------

class RenderPassBuilder {
public:
    RenderPassBuilder(Framegraph* fg, RenderPassBase* renderPass)
        : m_fg(fg)
        , m_renderPass(renderPass)
    {
    }

    template <typename ContentType>
    ContentType* read(ContentType* resource)
    {
        m_renderPass->m_reads.push_back(resource);
        resource->m_readers.push_back(m_renderPass);
        return resource;
    }

    template <typename ContentType>
    ContentType* read(const std::string& name)
    {
        ContentType* resource = m_fg->getResourceHandle<ContentType>(name);
        return read(resource);
    }

    template <typename ContentType>
    ContentType* write(ContentType* resource)
    {
        m_renderPass->m_writes.push_back(resource);
        resource->m_writers.push_back(m_renderPass);
        return resource;
    }

    template <typename ContentType>
    ContentType* write(const std::string& name)
    {
        ContentType* resource = m_fg->getResourceHandle<ContentType>(name);
        return write(resource);
    }

private:
    Framegraph* m_fg;
    RenderPassBase* m_renderPass;
};

//-------------------------------------------------------------------------------------------------

/*!
 * Implemenation of Framegraph function templates to resolve circular dependencies in gcc
 */
template <typename RenderPassData, typename... RenderPassArguments>
RenderPass<RenderPassData>* Framegraph::createRenderPass(RenderPassArguments&&... args)
{
    m_renderPasses.emplace_back(new RenderPass<RenderPassData>(m_renderPasses.size(), args...));
    RenderPassBase* renderPass = m_renderPasses.back().get();

    RenderPassBuilder builder(this, renderPass);
    renderPass->setup(builder);

    return static_cast<RenderPass<RenderPassData>*>(renderPass);
}

//-------------------------------------------------------------------------------------------------

template <typename ContentType, typename ContentDescriptionType>
void Framegraph::createResourceHandle(const std::string& name, int32_t resourceType, ContentDescriptionType& description)
{
    m_resourceHandleMap.insert(std::make_pair(name, new ContentType(name, resourceType, description)));
}

//-------------------------------------------------------------------------------------------------

template <typename ContentType>
ContentType* Framegraph::getResourceHandle(const std::string& name)
{
    return (ContentType*)m_resourceHandleMap[name].get();
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Framegraph_h
