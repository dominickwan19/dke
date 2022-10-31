//-------------------------------------------------------------------------------------------------
#ifndef DKE_GraphicsGL_GLShaderCreator_h_
#define DKE_GraphicsGL_GLShaderCreator_h_
//-------------------------------------------------------------------------------------------------
#include <graphics/Shader.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLShaderCreator : public dke::graphics::ShaderCreator {
public:
    graphics::Shader* createShader(int8_t shaderStageFlags, std::vector<std::string> shaderSources) override;

private:
    void getUniforms(graphics::Shader*& shader);
    void getUniformBlocks(graphics::Shader*& shader);
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // dke
//-------------------------------------------------------------------------------------------------
#endif
