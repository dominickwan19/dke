//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GeometryPass.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Camera.h>
#include <graphics/RenderCommand.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/StateManager.h>
#include <graphics_gl/GLRenderManager.h>
//-------------------------------------------------------------------------------------------------
#include <bitset>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
//-------------------------------------------------------------------------------------------------

namespace dke {
namespace graphics {
    template <>
    GLTextureObject* realizeResource(TextureDescription& description)
    {
        // Problem: It ignores the description now
        GLTextureObject* tex = nullptr;
        if (description.type == 0)
            tex = GLTextureObject::createRGBA8(2048, 2048);
        else if (description.type == 1)
            tex = GLTextureObject::createDEPTH24_STENCIL8(2048, 2048);

        return tex;
    }
}
}

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

typedef struct ShaderBuiltInData {
    glm::mat4 projMat;
    glm::mat4 viewMat;
};

//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------

GeometryPass::GeometryPass(int32_t index, RenderManager* renderManager)
    : RenderPass<GeometryPassData>(index, "GeometryPass", 0)
    , m_renderManager(renderManager)
{
}

//-------------------------------------------------------------------------------------------------

GeometryPass::~GeometryPass()
{
}

//-------------------------------------------------------------------------------------------------

void GeometryPass::setup(RenderPassBuilder& builder)
{
    data().outputColor[0] = builder.write<Texture2DResource>("normalMap");
    data().outputColor[1] = builder.write<Texture2DResource>("materialMap");
    data().outputColor[2] = builder.write<Texture2DResource>("surfaceColorMap");
    data().outputDepth = builder.write<Texture2DResource>("depth");
}

//-------------------------------------------------------------------------------------------------

void GeometryPass::clearBackground(const Colour4& colour)
{
    glClearDepthf(1.0f);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//-------------------------------------------------------------------------------------------------

void GeometryPass::execute(const Camera* camera, const RenderOptions* renderOptions)
{
    clearBackground(renderOptions->backgroundColour);

    const GeometryPassData& data = this->data();
    //int32_t currentIndex = currentFrame % 2;
    //std::vector<IFramebufferAttachable*> colorBuffers;
    //colorBuffers.push_back((GLTextureObject*)data.outputColor[0]->resource());
    //colorBuffers.push_back((GLTextureObject*)data.outputColor[1]->resource());
    //m_renderManager->m_fbo->attach(colorBuffers, (GLTextureObject*)data.outputDepth->resource());
    //m_renderManager->m_fbo->bind();

    ShaderBuiltInData builtIn;
    builtIn.projMat = camera->projectionMatrix();
    builtIn.viewMat = camera->viewMatrix();

    int32_t currentShaderId = -1;
    Shader* currentShader = nullptr;
    
    for (RenderCommand* rc : *getGLRenderManager()->getRenderQueue()) {
        if (rc->shaderId() == 0)
            continue;
        if (!rc->isActive())
            continue;

        if (currentShaderId != rc->shaderId()) {
            if (currentShader != nullptr)
                currentShader->unbind();

            currentShader = m_renderManager->getShaderManager()->get(rc->shaderId());
            currentShader->bind();
            currentShader->updateDataBlock("BuildIns", &builtIn, sizeof(builtIn));
            //currentShader->setUniform("viewportSize", viewport.width(), viewport.height()); // slow.

            currentShaderId = rc->shaderId();
        }

        rc->execute(m_renderManager, currentShader);
    }
}

//-------------------------------------------------------------------------------------------------

dke::graphics_gl::GLRenderManager* GeometryPass::getGLRenderManager()
{
    return dynamic_cast<dke::graphics_gl::GLRenderManager*>(m_renderManager);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
