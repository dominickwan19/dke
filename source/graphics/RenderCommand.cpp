//-------------------------------------------------------------------------------------------------
#include <graphics/RenderCommand.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderManager.h>
//-------------------------------------------------------------------------------------------------
#define INVALID_GEOMETRY_BUFFER_ID -1
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

static uint32_t g_currentRenderCommandID = 0;

//-------------------------------------------------------------------------------------------------

RenderCommand::RenderCommand()
    : m_id(g_currentRenderCommandID++)
    , m_key(0)
    , m_geometry(nullptr)
    , m_renderable(nullptr)
    , m_geometryBuffer(nullptr)
    , m_geometryBufferId(INVALID_GEOMETRY_BUFFER_ID)
    , m_shaderId(0)
    , m_textureId(0)
    , m_drawData(nullptr)
    , m_bActive(true)
    , m_useBlending(false)
    , m_useDepthTest(true)
    , m_useStencil(false)
    , m_bIsHighlighted(false)
{
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::execute(const RenderManager* renderManager, Shader* shader, RenderOptions* renderOptions)
{
    switch (drawData()->typeId()) {
    case RC_DRAW_ARRAYS:
        break;
    case RC_DRAW_ELEMENTS:
        break;
    default:
        break;
    }
}


//-------------------------------------------------------------------------------------------------

const Geometry::GeometryBufferHandle& RenderCommand::geometryHandle() const
{
    return m_geometryHandle;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setGeometryHandle(const Geometry::GeometryBufferHandle& h)
{
    m_geometryHandle = h;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setActive(bool active)
{
    m_bActive = active;
}

//-------------------------------------------------------------------------------------------------

bool RenderCommand::isActive() const
{
    return m_bActive;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setHighlightColour(const Colour4& colour)
{
    m_highlightColour = colour;
}

//-------------------------------------------------------------------------------------------------

const Colour4& RenderCommand::highlightColour() const
{
    return m_highlightColour;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setIsHighlighted(bool h)
{
    m_bIsHighlighted = h;
}

//-------------------------------------------------------------------------------------------------

bool RenderCommand::isHighlighted() const
{
    return m_bIsHighlighted;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setUseBlending(bool b)
{
    m_useBlending = b;
}

//-------------------------------------------------------------------------------------------------

bool RenderCommand::useBlending() const
{
    return m_useBlending;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setUseDepthTest(bool d)
{
    m_useDepthTest = d;
}

//-------------------------------------------------------------------------------------------------

bool RenderCommand::useDepthTest() const
{
    return m_useDepthTest;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setUseStencil(bool s)
{
    m_useStencil = s;
}

//-------------------------------------------------------------------------------------------------

bool RenderCommand::useStencil() const
{
    return m_useStencil;
}

//-------------------------------------------------------------------------------------------------

void RenderCommand::setDrawData(DrawData* pDrawData)
{
    m_drawData.reset(pDrawData);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
