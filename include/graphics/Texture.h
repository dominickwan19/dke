//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Texture_h
#define DKE_Graphics_Texture_h
//-------------------------------------------------------------------------------------------------

namespace dke {
namespace graphics {

// Interface for all texture objects
class Texture {
public:
    virtual ~Texture() = default;

    virtual int getId() = 0;

    virtual void create() = 0;
    virtual void destroy() = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual int getTarget() = 0; // wrap enum for texture type
    virtual void setTarget(int target) = 0;

    virtual int getFormat() = 0; // wrap internal format for texture type
    virtual void setFormat(int format) = 0;

    virtual int getInternalFormat() = 0;
    virtual void setInternalFormat(int internalFormat) = 0;

    virtual int getWidth() = 0;
    virtual void setWidth(int width) = 0;

    virtual int getHeight() = 0;
    virtual void setHeight(int height) = 0;

    virtual int getWrap(int wrapType) = 0;
    virtual void setWrap(int wrapType, int param) = 0;

    virtual int getFilter(int filterType) = 0;
    virtual void setFilter(int filterType, int param) = 0;

    virtual void setData(void* data) = 0;

protected:
    Texture() = delete;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Texture_h
