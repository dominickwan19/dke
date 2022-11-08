//-------------------------------------------------------------------------------------------------
#include <graphics/Renderable.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
#include <graphics/RenderCommand.h>
#include <graphics/RenderableKeyGenerator.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

Renderable::Renderable()
    : m_visible(true)
    , m_attributes()
{
    m_id = (uint32_t)((ptrdiff_t)this & 0xFFFFFFFF);
    DynamicObj::update();
}

//-------------------------------------------------------------------------------------------------

Renderable::~Renderable()
{
    m_geometries.clear();

    clearRenderCommands();
}

//-------------------------------------------------------------------------------------------------

void Renderable::setKey(uint64_t key)
{
    m_key = key;
}

//-------------------------------------------------------------------------------------------------

uint64_t Renderable::getKey()
{
    // update key fields
    m_attributes.translucency = transparencyFlag();

    // The renderable key is generated with attributes
    // from this renderable.
    DefaultRenderableKeyGenerator kg;
    m_key = kg.generateKey(this);
    return m_key;
}

//-------------------------------------------------------------------------------------------------

Renderable::CategoryType Renderable::category() const
{
    return (Renderable::CategoryType)m_attributes.category;
}

//-------------------------------------------------------------------------------------------------

ComponentType Renderable::component() const
{
    return (ComponentType)m_attributes.component;
}

//-------------------------------------------------------------------------------------------------

EntityType Renderable::entityType() const
{
    return (EntityType)m_attributes.entity;
}

//-------------------------------------------------------------------------------------------------

TranslucencyType Renderable::translucency() const
{
    return (TranslucencyType)m_attributes.translucency;
}

//-------------------------------------------------------------------------------------------------

void Renderable::setViewportId(int8_t id)
{
    m_attributes.viewportId = id;
}

//-------------------------------------------------------------------------------------------------

int8_t Renderable::viewportId() const
{
    return m_attributes.viewportId;
}

//-------------------------------------------------------------------------------------------------

int8_t Renderable::states() const
{
    return m_attributes.states;
}

//-------------------------------------------------------------------------------------------------

int32_t Renderable::order() const
{
    return m_attributes.depth;
}

//-------------------------------------------------------------------------------------------------

int32_t Renderable::material() const
{
    return m_attributes.material;
}

//-------------------------------------------------------------------------------------------------

void Renderable::setKeyStruct(CategoryType screenLayer, ComponentType component, EntityType entityType, TranslucencyType translucency)
{
    m_attributes = {};
    m_attributes.category = screenLayer;
    m_attributes.component = component;
    m_attributes.entity = entityType;
    m_attributes.translucency = translucency;
}

//-------------------------------------------------------------------------------------------------

void Renderable::setDepthId(int32_t depthId)
{
    m_attributes.depth = depthId;
}

//-------------------------------------------------------------------------------------------------

void Renderable::setMaterial(int32_t material)
{
    m_attributes.material = material;
}

//-------------------------------------------------------------------------------------------------

void Renderable::addGeometry(Geometry* geometry)
{
    if (geometry) {
        m_geometries.push_back(geometry);
        DynamicObj::update();
    }
}

//-------------------------------------------------------------------------------------------------

void Renderable::setRenderCommands(std::vector<RenderCommand*>& renderCommands)
{
    m_renderCommands.clear();
    m_renderCommands.insert(m_renderCommands.end(), renderCommands.begin(), renderCommands.end());
}

//-------------------------------------------------------------------------------------------------

void Renderable::addRenderCommand(RenderCommand* rc)
{
    m_renderCommands.push_back(rc);
}

//-------------------------------------------------------------------------------------------------

void Renderable::setVisible(bool bVisible)
{
    if (bVisible == visible())
        return;

    m_visible = bVisible;

    for (RenderCommand* rc : m_renderCommands) {
        const Renderable* renderable = rc->renderable();
        if (!renderable || renderable != this)
            continue;

        rc->setActive(m_visible);
    }
}

//-------------------------------------------------------------------------------------------------

bool Renderable::isUpdated(uint64_t refTimestamp)
{
    return isChangedSince(refTimestamp);
}

//-------------------------------------------------------------------------------------------------

void Renderable::updateRenderCommands(RenderableKeyGenerator* keyGen)
{
    // m_renderCommands.clear();
    clearRenderCommands();

    for (size_t i = 0; i < m_geometries.size(); ++i) {
        RenderCommand* rc = new RenderCommand();
        rc->setRenderable(this);

        rc->setGeometry(m_geometries[i]);
        rc->setKey(getKey());

        m_renderCommands.push_back(rc);
    }
}

//-------------------------------------------------------------------------------------------------

void Renderable::clearRenderCommands()
{
    // for (auto r : m_renderCommands)
    //     delete r;
    m_renderCommands.clear();
}

//-------------------------------------------------------------------------------------------------

size_t Renderable::numGeometryHandles() const
{
    return m_geometryHandles.size();
}

//-------------------------------------------------------------------------------------------------

void Renderable::setGeometryHandle(int index, const Geometry::GeometryBufferHandle& h)
{
    if (m_geometryHandles.size() > index)
        m_geometryHandles[index] = h;
    else
        m_geometryHandles.push_back(h);
}

//-------------------------------------------------------------------------------------------------

const Geometry::GeometryBufferHandle& Renderable::getGeometryHandle(int index) const
{
    return m_geometryHandles[index];
}

//-------------------------------------------------------------------------------------------------

void Renderable::invalidateGeometryHandles()
{
    m_geometryHandles.clear();
}

//-------------------------------------------------------------------------------------------------

void Renderable::clearGeometryData()
{
    for (auto& g : m_geometries) {
        if (g->flags() == GEOMETRY_FLAGS_STATIC)
            g->clearData();
    }
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
