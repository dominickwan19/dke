//-------------------------------------------------------------------------------------------------
#ifndef DKE_GraphicsGL_GLRenderable_h
#define DKE_GraphicsGL_GLRenderable_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Renderable.h>
#include <functional>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLRenderable : public dke::graphics::Renderable {
public:
    GLRenderable();

    void initialize(dke::graphics::RenderManager* renderManager);

//    virtual void update() override;

    virtual void onGeometryUpdate(std::vector<std::unique_ptr<dke::graphics::Geometry>>& geometries) override;
    virtual void onBindParameters(dke::graphics::RenderCommand* rc, dke::graphics::Shader* shader) override;
    void setBindParamFunction(const std::function<void(graphics::RenderCommand* rc, graphics::Shader* shader)>& bindParamFunc) { m_bindParamFunc = bindParamFunc; }

private:
    dke::graphics::RenderManager* m_pRenderManager;
    std::function<void(graphics::RenderCommand* rc, graphics::Shader* shader)> m_bindParamFunc;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_Gl
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // DKE_GraphicsGL_GLRenderable_h
