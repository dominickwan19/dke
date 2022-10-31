//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_MaterialManager_h
#define DKE_Graphics_MaterialManager_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Material.h>
#include <graphics/Image.h>
#include <vector>
#include <unordered_map>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class MaterialManager {
public:
    MaterialManager() = default;
    ~MaterialManager()
    {
        for (auto* m : m_materials)
            delete m;
    }

    void addMaterial(MaterialBase* material) { m_materials.push_back(material); }

private:
    std::vector<MaterialBase*> m_materials;
};

class ITextureObject;

class TextureManager {
public:
    TextureManager() = default;
    virtual ~TextureManager();

    void addPendingTexture(const std::string& name, const std::string& filePath)
    {
        m_pendingTextureMap.insert(std::make_pair(name, filePath));
    }

    ITextureObject* findTexture(const std::string& name) const
    {
        auto itr = m_textureMap.find(name);
        return (itr != m_textureMap.end()) ? itr->second : nullptr;
    }
    ITextureObject* findTexture(int32_t id) const
    {
        auto itr = m_textureIdMap.find(id);
        return (itr != m_textureIdMap.end()) ? itr->second : nullptr;
    }

    virtual void loadPendingTextures() {}

    virtual int32_t loadTexture(Image& image) { return 0; }

protected:
    std::unordered_map<std::string, std::string> m_pendingTextureMap;
    std::unordered_map<std::string, ITextureObject*> m_textureMap;
    std::unordered_map<int32_t, ITextureObject*> m_textureIdMap;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_MaterialManager_h
