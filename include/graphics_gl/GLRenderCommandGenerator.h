//-------------------------------------------------------------------------------------------------
#ifndef DKE_GraphicsGL_GLRenderCommandGenerator_h
#define DKE_GraphicsGL_GLRenderCommandGenerator_h
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderCommand.h>
#include <functional>
//-------------------------------------------------------------------------------------------------

// Forward Declarations.
namespace dke {
namespace graphics {
class Renderable;
class RenderManager;
class Buffer;
class UniformBuffer;
} // namespace graphics
}

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLRenderCommandGenerator {//: public graphics::RenderCommandGenerator {
public:
    GLRenderCommandGenerator(graphics::RenderManager* renderManager);

    std::vector<graphics::RenderCommand*> generate(graphics::Renderable* renderable, std::vector<std::unique_ptr<graphics::Geometry>>& geometries) const;

protected:
    graphics::RenderCommand* generateLines(uint64_t key, graphics::Renderable* renderable, graphics::Geometry* geometry, graphics::UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const;
    graphics::RenderCommand* generateLineStrip(uint64_t key, graphics::Renderable* renderable, graphics::Geometry* geometry, graphics::UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const;
    graphics::RenderCommand* generateMesh(uint64_t key, graphics::Renderable* renderable, graphics::Geometry* geometry, graphics::UniformBuffer* uniformBuffer, int32_t offset, int32_t size) const;

private:
    graphics::RenderManager* m_renderManager;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke 
//-------------------------------------------------------------------------------------------------

#endif // GLRenderCommandGenerator_h
