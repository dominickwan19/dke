//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLRenderable.h>
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

GLRenderable::GLRenderable()
    : Renderable()
{
}

//-------------------------------------------------------------------------------------------------

void GLRenderable::onBindParameters(RenderCommand* rc, Shader* shader)
{
    if (m_bindParamFunc)
        m_bindParamFunc(rc, shader);
}

//-------------------------------------------------------------------------------------------------

void GLRenderable::onGeometryUpdate(std::vector<std::unique_ptr<Geometry>>& geometries)
{
    for (auto g : m_geometries)
        geometries.push_back(std::unique_ptr<Geometry>(std::move(g)));
    m_geometries.clear();
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
