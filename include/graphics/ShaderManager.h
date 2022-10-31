//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_ShaderManager_h
#define DKE_Graphics_ShaderManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Shader.h>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class ShaderManager {
public:
    ShaderManager();
    virtual ~ShaderManager() {}

    uint32_t create(const std::vector<std::string>& shaderSources, int shaderType, int8_t shaderStageFlags = 0);
    Shader* find(int shaderType) const;
    Shader* get(uint32_t id) const;
    void clear();

    void setShaderCreator(ShaderCreator* creator);

private:
    std::unordered_map<int, std::unique_ptr<Shader>> m_shaderMap;
    std::unordered_map<uint32_t, Shader*> m_shaderIdMap;
    std::unique_ptr<ShaderCreator> m_shaderCreator;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_ShaderManager_h
