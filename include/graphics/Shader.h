//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Shader_h
#define DKE_Graphics_Shader_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Colour.h>
#include <map>
#include <vector>
#include <string>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Shader;

class ShaderCreator {
public:
    virtual ~ShaderCreator() { }
    virtual Shader* createShader(int8_t shaderStageFlags, std::vector<std::string> shaderSources) = 0;
};

//-------------------------------------------------------------------------------------------------

class Shader {
public:
    struct DataBlockInfo {
        DataBlockInfo()
            : index(-1)
        {
        }

        DataBlockInfo(uint32_t index, const std::string& name, uint32_t blockSize, uint32_t bindingPoint)
            : index(index)
            , name(name)
            , size(blockSize)
            , bindingPoint(bindingPoint)
        {
        }

        std::string name;
        int32_t index;
        uint32_t size;
        uint32_t bindingPoint;
    };

public:
    Shader()
        : m_id(0)
    {
    }

    virtual ~Shader() = default;

    virtual void bind() { }
    virtual void unbind() { }

    virtual void setUniform(const char* uniformName, void* val) { }
    virtual void setUniform(const char* uniformName, int val) { }
    virtual void setUniform(const char* uniformName, float v0) { }
    virtual void setUniform(const char* uniformName, float v0, float v1) { }
    virtual void setUniform(const char* uniformName, float v0, float v1, float v2) { }
    virtual void setUniform(const char* uniformName, float v0, float v1, float v2, float v3) { }
    virtual void setUniform(const char* uniformName, const Colour4& colour) { setUniform(uniformName, colour.r, colour.g, colour.b, colour.a); }
    virtual void setUniformMatrix4(const char* uniformName, void* val) { }

    virtual void setDataBinding(uint32_t dataBlockIndex, const std::string& dataBlockName, uint32_t dataBufferHandle, uint32_t blockSize);
    virtual void setDataBlockInfo(uint32_t blockIndex, const std::string& blockName, uint32_t blockSize, uint32_t bindingPoint);
    virtual const DataBlockInfo& getDataBlockInfo(uint32_t blockIndex);
    virtual const DataBlockInfo& getDataBlockInfo(const std::string& blockName);

    // to be removed. The buffer will be updated outside. This class only needs to know the binding point.
    virtual void updateDataBlock(const std::string& dataBlockName, void* val, size_t dataSize) { }

    uint32_t id() { return m_id; }
    void setId(uint32_t id) { m_id = id; }

protected:
    // shader handle -
    uint32_t m_id;

    // TO BE REMOVED
    struct DataBinding {
        DataBinding(uint32_t index, const std::string& name, uint32_t handle, uint32_t blockSize)
            : blockIndex(index)
            , name(name)
            , dataBufferHandle(handle)
            , blockSize(blockSize)
        {
        }

        uint32_t blockIndex; // block index in the shader
        std::string name; // block name defined in shader
        uint32_t dataBufferHandle; // handle to UBO. To be removed.
        uint32_t blockSize; // size of this block in bytes, with possible alighmemt/padding accounted in the value
        uint32_t bindingPoint; // binding point
    };

    std::map<int32_t, DataBinding> m_uniformDataBindings; // TO BE REMOVED
    std::map<int32_t, DataBlockInfo> m_uniformBlockInfoMap; // block index to block info mapping

public:
    static const DataBlockInfo m_nullDataBlock;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Shader_h
