//-------------------------------------------------------------------------------------------------
#include <graphics/framegraph/Framegraph.h>
//-------------------------------------------------------------------------------------------------
#include <algorithm>
#include <set>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

struct ResourceHandleStartEnd {
    ResourceHandleBase* resourceHandle;
    int32_t start;
    int32_t end;

    ResourceHandleStartEnd(ResourceHandleBase* r, int32_t s, int32_t e)
        : resourceHandle(r)
        , start(s)
        , end(e)
    {
    }
};

//-------------------------------------------------------------------------------------------------

bool operator<(const ResourceHandleStartEnd& lhs, const ResourceHandleStartEnd& rhs)
{
    return lhs.resourceHandle->name() < rhs.resourceHandle->name();
}

//-------------------------------------------------------------------------------------------------

RenderPassBase* Framegraph::addRenderPass(RenderPassBase* renderPass)
{
    m_renderPasses.emplace_back(renderPass);

    RenderPassBuilder builder(this, renderPass);
    renderPass->setup(builder);

    return renderPass;
}

//-------------------------------------------------------------------------------------------------

void Framegraph::execute(const Camera* camera, const RenderOptions* renderOptions)
{
    for (const auto& r : m_renderPasses)
        r->execute(camera, renderOptions);
}

//-------------------------------------------------------------------------------------------------

void Framegraph::compile()
{
    std::vector<ResourceHandleBase*> resourceHandles;
    for (const auto& r : m_resourceHandleMap) {
        resourceHandles.push_back(r.second.get());
    }

    // For all resource, build their life span indices. Start value
    // denotes the first passes the resource handle is reference,
    // and end value denotes the last
    std::vector<std::pair<int32_t, int32_t>> resourceStartEndValues(resourceHandles.size());
    for (int i = 0; i < resourceHandles.size(); i++) {
        auto resourceHandle = resourceHandles[i];

        int32_t startIndex = 100;
        if (!resourceHandle->m_readers.empty())
            startIndex = std::min<int32_t>(startIndex, resourceHandle->m_readers[0]->index());
        if (!resourceHandle->m_writers.empty())
            startIndex = std::min<int32_t>(startIndex, resourceHandle->m_writers[0]->index());
        int32_t endIndex = -1;
        if (!resourceHandle->m_readers.empty())
            endIndex = resourceHandle->m_readers[resourceHandle->m_readers.size() - 1]->index();
        if (!resourceHandle->m_writers.empty())
            endIndex = std::max<int32_t>(endIndex, resourceHandle->m_writers[resourceHandle->m_writers.size() - 1]->index());

        resourceStartEndValues[i].first = startIndex;
        resourceStartEndValues[i].second = endIndex;
#ifdef _DEBUG
        printf("Resource handle: name=%s start=%d end=%d\n", resourceHandle->name().c_str(), startIndex, endIndex);
#endif
    }

    // Put active resource into a set for use later
    std::set<ResourceHandleStartEnd> resourceHandleSet;
    for (int i = 0; i < resourceHandles.size(); i++) {
        auto resourceHandle = resourceHandles[i];

        // if resource is not valid (not referenced at all), skip it
        if (!(resourceStartEndValues[i].first >= 0 && resourceStartEndValues[i].second >= 0))
            continue;

        resourceHandleSet.emplace(resourceHandle, resourceStartEndValues[i].first, resourceStartEndValues[i].second);
    }

    // ========================================================================
    // Build a list of resource, where each of them will be referenced by resource handles.
    // Here we start with array storing another array of handle associated for each resource.
    //
    // The lists are filled up by resource handle that fits into remaining empty
    // space of the list from front to back.
    // ========================================================================

    int pipelineSize = (int32_t)m_renderPasses.size();
    int currentIndex = 0; // current position in the list for looking up a suitable resource handle to fit in
    m_resourceList.clear();
    // start with the array of handles for an active resource
    m_resourceList.push_back(std::vector<ResourceHandleBase*>());
    // repeat until all resource in resourceHandleSet are placed into the list
    while (!resourceHandleSet.empty()) {
        int type = 0;
        if (m_resourceList.back().size() > 0) {
            type = m_resourceList.back().front()->resourceType();
        } else {
            type = resourceHandleSet.begin()->resourceHandle->resourceType();
        }

        // find the minimum start value from resourceHandleSet that is bigger than currentIndex
        int minValue = pipelineSize;
        for (auto r : resourceHandleSet) {
            if (r.start >= currentIndex && type == r.resourceHandle->resourceType())
                minValue = std::min(minValue, r.start);
        }
#ifdef _DEBUG
        printf("current index: %d\n", currentIndex);
        printf("minValue: %d\n", minValue);
#endif
        // with minValue, find the iterator pointing to that element in set
        auto it = std::find_if(resourceHandleSet.begin(), resourceHandleSet.end(), [&](const ResourceHandleStartEnd& val) -> bool {
            // two conditions to met: the handle's start value is the value we want,
            // and the handle can fit into remaining free size
            // TODO: third condition is that texture format matches
            bool a = val.resourceHandle->resourceType() == type;
            bool b = val.start == minValue;
            bool c = (val.end - val.start + currentIndex) < pipelineSize;
            return a && b && c;
        });

        // if nothing is found but we will have handle in set,
        // start a new list (adding another new resource)
        if (it == resourceHandleSet.end()) {
            m_resourceList.push_back(std::vector<ResourceHandleBase*>());
            currentIndex = 0;
            continue;
        }
#ifdef _DEBUG
        printf("resrouce: %s\n", (*it).resourceHandle->name().c_str());
#endif

        // add resource handle into the list
        m_resourceList.back().emplace_back((*it).resourceHandle);
        // advance currentIndex
        currentIndex = (*it).end + 1;
        // remove this handle from set
        resourceHandleSet.erase(it);
    }

    // Allocate resources based on the list.
    // The implementation is a bit bad here - it looks like the first
    // resource handle has ownership of the resource.
    // Proper implementation would be asking for a resource manager
    // or the framegraph to realize and hold the resource, but
    // unfortunately the template based implementation means
    // ContentType and ContentDescriptionType of resource is not
    // known to others. Might review this later.
    for (const auto& l : m_resourceList) {
        const auto& resourceHandle = l.front();
        auto resource = resourceHandle->realize();

        // assign pointer to others
        for (const auto& resourceHandle : l) {
            resourceHandle->setResource(resource);
        }
    }
}

// not sure if we still need a "timeline" here since all the resources
// are allocated and assigned to handles already.
//void Framegraph::compile() {
//    // for all fg resources, find start and end index
//    struct RenderStep {
//        int renderPassIndex;
//        std::vector<std::string> realized;
//        std::vector<std::string> derealized;
//
//        RenderStep(int i) : renderPassIndex(i) {
//
//        }
//    };
//
//    std::vector<RenderStep> timeline;
//    for (int i = 0; i < m_renderPasses.size(); i++) {
//        timeline.emplace_back(i);
//    }
//    for (auto resource : m_resources) {
//        int32_t minIndex = -1;
//        minIndex = resource->m_creator->index();
//        //if (!resource->m_readers.empty())
//        //    minIndex = resource->m_readers[0]->index();
//        //if (!resource->m_writers.empty())
//        //    minIndex = std::min<int32_t>(minIndex, resource->m_writers[0]->index());
//
//        int32_t maxIndex = -1;
//        if (!resource->m_readers.empty())
//            maxIndex = resource->m_readers[resource->m_readers.size() - 1]->index();
//        if (!resource->m_writers.empty())
//            maxIndex = std::max<int32_t>(maxIndex, resource->m_writers[resource->m_writers.size() - 1]->index());
//
//        // with min max, we can know which render pass should realize the resource, and which render pass should derealize
//        if (minIndex != -1 && maxIndex != -1) {
//            timeline[minIndex].realized.push_back(resource->name());
//            timeline[maxIndex].derealized.push_back(resource->name());
//        }
//
//        printf("min: %d, max: %d\n", minIndex, maxIndex);
//    }
//
//    for (auto step : timeline) {
//        printf("pass: %d ", step.renderPassIndex);
//        printf("realized: ");
//        for (auto r : step.realized) {
//            printf("%s ", r.c_str());
//        }
//        printf("derealized: ");
//        for (auto d : step.derealized) {
//            printf("%s ", d.c_str());
//        }
//        printf("\n");
//    }
//}

//-------------------------------------------------------------------------------------------------
} // graphics
} // dke
//-------------------------------------------------------------------------------------------------
