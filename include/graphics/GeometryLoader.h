//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_GeometryLoader_h
#define DKE_Graphics_GeometryLoader_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Buffer.h>
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------
#include <functional>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class MeshGeometry;

/**
    Load geometry from store.
*/
class GeometryLoader {
public:
    GeometryLoader();
    ~GeometryLoader();

    template <typename T>
    void loadFromFile(std::vector<T*>& geometry, const std::string& filename);
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_GeometryLoader_h
