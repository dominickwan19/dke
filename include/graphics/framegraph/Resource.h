//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_ResourceBase_h
#define DKE_Graphics_ResourceBase_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Framegraph;
class RenderPassBase;
class RenderPassBuilder;

class ResourceBase {
public:
    ResourceBase(int type)
        : m_type(type)
    {
        static int32_t id = 0;
        m_id = id++;
    }
    virtual ~ResourceBase() = default;

    int32_t id() const
    {
        return m_id;
    }

    int32_t type() const
    {
        return m_type;
    }

private:
    int32_t m_id;
    int32_t m_type;
};

class ResourceHandleBase {
public:
    ResourceHandleBase(const std::string& name, int32_t resourceType)
        : m_name(name)
        , m_resource(nullptr)
        , m_resourceType(resourceType)
        , m_refCount(0)
    {
        static int32_t id = 0;
        m_id = id++;
    }
    virtual ~ResourceHandleBase() = default;

    int32_t id() const
    {
        return m_id;
    }
    const std::string& name() const
    {
        return m_name;
    }
    bool transient() const
    {
        return false;
    }

    virtual ResourceBase* realize() = 0;

    void setResource(ResourceBase* resource)
    {
        m_resource = resource;
    }

    ResourceBase* resource()
    {
        return m_resource;
    }

    int32_t resourceType()
    {
        return m_resourceType;
    }

protected:
    friend Framegraph;
    friend RenderPassBuilder;

    int32_t m_id;
    std::string m_name;
    std::vector<const RenderPassBase*> m_readers;
    std::vector<const RenderPassBase*> m_writers;
    int32_t m_refCount;

    //    std::weak_ptr<ResourceBase> m_resource;
    int32_t m_resourceType;
    ResourceBase* m_resource;
    std::unique_ptr<ResourceBase> m_resourceOwner;
};

template <typename ContentType, typename ContentDescriptionType>
ContentType* realizeResource(ContentDescriptionType& description)
{
    return nullptr;
}

template <typename ContentType, typename ContentDescriptionType>
class ResourceHandle : public ResourceHandleBase {
public:
    ResourceHandle(const std::string& name, int32_t resourceType, const ContentDescriptionType& description)
        : ResourceHandleBase(name, resourceType)
        , m_description(description)
    {
    }
    ~ResourceHandle() = default;

    ResourceBase* realize() override
    {
        m_resourceOwner.reset(realizeResource<ContentType>(m_description));
        return m_resourceOwner.get();
    }

private:
    ContentDescriptionType m_description;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_ResourceBase_h
