//-------------------------------------------------------------------------------------------------
#ifndef DKE_GraphicsGL_GLShader_h_
#define DKE_GraphicsGL_GLShader_h_
//-------------------------------------------------------------------------------------------------
#include <graphics/Shader.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

class GLShader : public dke::graphics::Shader {
public:
    GLShader() = default;
    virtual ~GLShader() = default;

    void bind() override;
    void unbind() override;

    void setUniform(const char* uniformName, void* val) override;
    void setUniform(const char* uniformName, int32_t val) override;
    void setUniform(const char* uniformName, float v0) override;
    void setUniform(const char* uniformName, float v0, float v1) override;
    void setUniform(const char* uniformName, float v0, float v1, float v2) override;
    void setUniform(const char* uniformName, float v0, float v1, float v2, float v3) override;
    void setUniformMatrix4(const char* uniformName, void* val) override;
    //void setUniformBlock() override;

    void setDataBinding(uint32_t dataBlockIndex, const std::string& dataBlockName, uint32_t dataBufferHandle, uint32_t blockSize) override;
    void updateDataBlock(const std::string& dataBlockName, void* val, size_t dataSize) override;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // GLShader
