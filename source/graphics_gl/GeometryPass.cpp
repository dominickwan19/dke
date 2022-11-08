//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GeometryPass.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Camera.h>
#include <graphics/RenderCommand.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics/StateManager.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics_gl/GLTextureObject.h>
//-------------------------------------------------------------------------------------------------
#include <bitset>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
using namespace dke::graphics_gl;
//-------------------------------------------------------------------------------------------------

namespace dke {
namespace graphics {
    template <>
    GLTextureObject* realizeResource(TextureDescription& description)
    {
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
    const GeometryPassData& data = this->data();

    std::vector<IFramebufferAttachable*> colorBuffers;
    colorBuffers.push_back((GLTextureObject*)data.outputColor[0]->resource());
    colorBuffers.push_back((GLTextureObject*)data.outputColor[1]->resource());
    auto a = (GLTextureObject*)data.outputDepth->resource();
    getGLRenderManager()->getFBO()->attach(colorBuffers, (GLTextureObject*)data.outputDepth->resource(), true);
    getGLRenderManager()->getFBO()->bind();

    // TODO: built this somewhere else, pass into render passes
    ShaderBuiltInData builtIn;
    builtIn.projMat = camera->projectionMatrix();
    builtIn.viewMat = camera->viewMatrix();
    builtIn.normalMat = glm::inverse(glm::transpose(camera->viewMatrix()));

    int32_t currentShaderId = -1;
    Shader* currentShader = nullptr;

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    clearBackground(renderOptions->backgroundColour);

    for (RenderCommand* rc : *getGLRenderManager()->getRenderQueue()) {
        if (rc->shaderId() == 0)
            continue;
        if (!rc->isActive())
            continue;

        if (rc->renderable()->category() != Renderable::CATEGORY_GEOMETRY)
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

    getGLRenderManager()->getFBO()->unbind();

    //glDepthMask(GL_TRUE);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //getGLRenderManager()->getFBO()->blitToBuffer(0, 0);
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
