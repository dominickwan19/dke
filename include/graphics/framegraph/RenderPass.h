//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderPass_h
#define DKE_Graphics_RenderPass_h
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderOptions.h>
//-------------------------------------------------------------------------------------------------
#include <functional>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Camera;
class Framegraph;
class ResourceHandleBase;
class RenderPassBuilder;

//-------------------------------------------------------------------------------------------------

class RenderPassBase {
public:
    RenderPassBase(int32_t index, const std::string& name, uint64_t key)
        : m_index(index)
        , m_name(name)
        , m_key(key)
        , m_refCount(0)
    {
    }
    virtual ~RenderPassBase() {}

    const std::string& name() const { return m_name; }
    int32_t index() const { return m_index; }

protected:
    friend Framegraph;
    friend RenderPassBuilder;

    virtual void setup(RenderPassBuilder& builder) = 0;
    virtual void execute(const Camera* camera, const RenderOptions* renderOptions) = 0;

private:
    int32_t m_index;
    uint64_t m_key;
    std::string m_name;
    std::vector<const ResourceHandleBase*> m_creates;
    std::vector<const ResourceHandleBase*> m_reads;
    std::vector<const ResourceHandleBase*> m_writes;
    int32_t m_refCount;
};

//-------------------------------------------------------------------------------------------------

template <typename RenderPassData>
class RenderPass : public RenderPassBase {
public:
    RenderPass(int32_t index, const std::string& name, uint64_t key, const std::function<void(RenderPassData&, RenderPassBuilder& builder)> setup = nullptr, const std::function<void(const RenderPassData&)> execute = nullptr)
        : RenderPassBase(index, name, key)
        , m_setup(setup)
        , m_execute(execute)
    {
    }

    const RenderPassData& data() const { return m_data; }

protected:
    virtual void setup(RenderPassBuilder& builder) { m_setup(m_data, builder); }
    virtual void execute(const Camera* camera, const RenderOptions* renderOPtions) { m_execute(m_data); }

    RenderPassData& data() { return m_data; }

private:
    RenderPassData m_data;
    const std::function<void(RenderPassData&, RenderPassBuilder& builder)> m_setup;
    const std::function<void(const RenderPassData&)> m_execute;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderPass_h
