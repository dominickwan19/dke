#include <graphics/Shader.h>
//-------------------------------------------------------------------------------------------------
#include <fstream>
#include <string>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

const Shader::DataBlockInfo Shader::m_nullDataBlock = Shader::DataBlockInfo();

void Shader::setDataBinding(uint32_t dataBlockIndex, const std::string& dataBlockName, uint32_t dataBufferHandle, uint32_t blockSize)
{
    m_uniformDataBindings.insert(std::make_pair(dataBlockIndex, DataBinding(dataBlockIndex, dataBlockName, dataBufferHandle, blockSize)));
}

//-------------------------------------------------------------------------------------------------

void Shader::setDataBlockInfo(uint32_t blockIndex, const std::string& blockName, uint32_t blockSize, uint32_t bindingPoint)
{
    DataBlockInfo blockInfo(blockIndex, blockName, blockSize, bindingPoint);
    m_uniformBlockInfoMap.insert(std::make_pair(blockIndex, blockInfo));
}

//-------------------------------------------------------------------------------------------------

const Shader::DataBlockInfo& Shader::getDataBlockInfo(uint32_t blockIndex)
{
    std::map<int32_t, DataBlockInfo>::iterator it = m_uniformBlockInfoMap.find(blockIndex);
    if (it != m_uniformBlockInfoMap.end())
        return it->second;

    return m_nullDataBlock;
}

//-------------------------------------------------------------------------------------------------

const Shader::DataBlockInfo& Shader::getDataBlockInfo(const std::string& blockName)
{
    for (const auto& i : m_uniformBlockInfoMap) {
        if (i.second.name == blockName)
            return i.second;
    }

    return m_nullDataBlock;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
