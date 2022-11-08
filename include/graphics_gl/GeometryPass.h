//-------------------------------------------------------------------------------------------------
#ifndef GeometryPass_h
#define GeometryPass_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Colour.h>
#include <graphics/RenderManager.h>
#include <graphics/framegraph/RenderPass.h>
#include <graphics/framegraph/Resource.h>
#include <graphics_gl/GLTextureObject.h>
#include <glm/mat4x4.hpp> // glm::mat4
//-------------------------------------------------------------------------------------------------
#include <set>
//-------------------------------------------------------------------------------------------------

namespace dke {
namespace graphics_gl {
    class GLRenderManager;
    class GLTextureObject;
}
}

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

struct TextureDescription {
    int32_t type;
    int32_t width;
    int32_t height;

    TextureDescription(int32_t type = 0, int32_t width = 1024, int32_t height = 1024)
        : type(type)
        , width(width)
        , height(height)
    {
    }
};

//-------------------------------------------------------------------------------------------------

template <>
dke::graphics_gl::GLTextureObject* dke::graphics::realizeResource(TextureDescription& description);

//-------------------------------------------------------------------------------------------------

using Texture2DResource = dke::graphics::ResourceHandle<dke::graphics_gl::GLTextureObject, TextureDescription>;

//-------------------------------------------------------------------------------------------------

typedef struct ShaderBuiltInData {
    glm::mat4 projMat;
    glm::mat4 viewMat;
    glm::mat4 normalMat;
} ShaderBuiltInData;

//-------------------------------------------------------------------------------------------------

struct GeometryPassData {
    //std::vector<Texture2DResource*> outputColor;
    Texture2DResource* outputColor[3];
    Texture2DResource* outputDepth;
};

//-------------------------------------------------------------------------------------------------

class GeometryPass : public dke::graphics::RenderPass<GeometryPassData> {
public:
    GeometryPass(int32_t index, dke::graphics::RenderManager* renderManager);
    ~GeometryPass();

    dke::graphics_gl::GLRenderManager* getGLRenderManager();

protected:
    virtual void setup(dke::graphics::RenderPassBuilder& builder) override;
    virtual void execute(const dke::graphics::Camera* camera, const dke::graphics::RenderOptions* renderOptions) override;

    void clearBackground(const dke::graphics::Colour4& clearColour);

private:
    dke::graphics::RenderManager* m_renderManager;
};

//-------------------------------------------------------------------------------------------------
} // graphics
}
//-------------------------------------------------------------------------------------------------

#endif // GeometryPass_h
