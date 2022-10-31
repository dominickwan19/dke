//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Material_h
#define DKE_Graphics_Material_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Material is the abstract concept of surface properties, so we
// are decoupling surface data from actual shaders (which are used
// for calculation of result)
// just implement this in a straight forward way now - create fields
// that maps to our shader. Should find a generic solution

class MaterialBase {
public:
    virtual ~MaterialBase();

    void* getData() { return nullptr; }
    size_t getDataSize() { return 0; }

public:
    std::vector<int32_t> textureIds;
};

template <typename T>
class Material : public MaterialBase {
public:
    Material()
        : m_data(new T)
    {
    }
    T* getData() { return m_data; }

private:
    T* m_data;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Material_h
