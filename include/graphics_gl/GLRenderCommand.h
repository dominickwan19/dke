//-------------------------------------------------------------------------------------------------
#ifndef DKE_GraphicsGL_GLRenderCommand_h
#define DKE_GraphicsGL_GLRenderCommand_h
//-------------------------------------------------------------------------------------------------
#include <graphics/RenderCommand.h>
#include <graphics/RenderManager.h>
#include <graphics/Shader.h>
#include <graphics/UniformBuffer.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

//graphics::RenderCommand* createGLCommand(int32_t type = 0);

//-------------------------------------------------------------------------------------------------

class GLRenderCommand : public graphics::RenderCommand {
public:
    GLRenderCommand();

    virtual void execute(const graphics::RenderManager* renderManager, graphics::Shader* shader, graphics::RenderOptions* renderOptions = nullptr) override;

    void setUniformBuffer(graphics::UniformBuffer* ub);
    void setInstanceData(int32_t offset, int32_t size);

private:
    int32_t m_instanceDataOffset;
    int32_t m_instanceDataSize;
    graphics::UniformBuffer* m_uniformBuffer;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // GLRenderCommand_h
