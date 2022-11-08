//-------------------------------------------------------------------------------------------------
#ifndef MeshGeometry_h
#define MeshGeometry_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
    //-------------------------------------------------------------------------------------------------

    class MeshGeometry : public graphics::Geometry {
    private:
        enum {
            ATTR_POSITION = 0,
            ATTR_NORMAL = 1,
            ATTR_COLOUR = 2
        };

    public:
        MeshGeometry();
        virtual ~MeshGeometry();

        inline void addPosition(float* data, size_t n) { addAttributeData<float>(ATTR_POSITION, data, n); }
        inline void addNormal(float* data, size_t n) { addAttributeData<float>(ATTR_NORMAL, data, n); }
        inline void addColour(float* data, size_t n) { addAttributeData<float>(ATTR_COLOUR, data, n); }
        //inline void addUV(float* data, size_t n) { addAttributeData<float>(ATTR_UV, data, n); }
    };

    //-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------

#endif // MeshGeometry_h
