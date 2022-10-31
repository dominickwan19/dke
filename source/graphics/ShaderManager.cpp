//-------------------------------------------------------------------------------------------------
#include <graphics/ShaderManager.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Shader.h>
#include <assert.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

ShaderManager::ShaderManager()
{
}

//-------------------------------------------------------------------------------------------------

uint32_t ShaderManager::create(const std::vector<std::string>& shaderSources, int shaderType, int8_t shaderStageFlags)
{
    // Create shader with a factory?
    assert(m_shaderCreator);
    Shader* shader = m_shaderCreator->createShader(shaderStageFlags, shaderSources);

    // insert to map
    m_shaderMap.emplace(shaderType, shader);
    m_shaderIdMap.emplace(shader->id(), shader);

    return shader->id();
}

//-------------------------------------------------------------------------------------------------

Shader* ShaderManager::find(int shaderType) const
{
    auto it = m_shaderMap.find(shaderType);
    if (it != m_shaderMap.end())
        return it->second.get();

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

Shader* ShaderManager::get(uint32_t id) const
{
    return m_shaderIdMap.at(id);
}

//-------------------------------------------------------------------------------------------------

void ShaderManager::clear()
{
    m_shaderIdMap.clear();
    m_shaderMap.clear();
}

//-------------------------------------------------------------------------------------------------

void ShaderManager::setShaderCreator(ShaderCreator* creator)
{
    m_shaderCreator.reset(creator);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
