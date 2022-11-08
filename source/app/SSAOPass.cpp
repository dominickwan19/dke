//-------------------------------------------------------------------------------------------------
#include <app/SSAOPass.h>
//-------------------------------------------------------------------------------------------------
#include <graphics/Camera.h>
#include <graphics/Renderable.h>
#include <graphics/RenderCommand.h>
#include <graphics/RenderableKeyGenerator.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics_gl/GLRenderCommandGenerator.h>
#include <graphics_gl/GLRenderCommand.h>
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
using namespace dke::graphics_gl;
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

SSAOPass::SSAOPass(int32_t index, RenderManager* renderManager, Renderable* screenQuadRenderable)
    : RenderPass<SSAOPassData>(index, "SSAOPass", 0)
    , m_renderManager(renderManager)
    , m_screenQuadRenderable(screenQuadRenderable)
{
}

//-------------------------------------------------------------------------------------------------

SSAOPass::~SSAOPass()
{
}

//-------------------------------------------------------------------------------------------------

void SSAOPass::setup(RenderPassBuilder& builder)
{
    data().inputColour[0] = builder.read<Texture2DResource>("normalMap");
    data().inputColour[1] = builder.read<Texture2DResource>("depth");
    data().outputColor[0] = builder.write<Texture2DResource>("ssaoMap");
    data().outputDepth = builder.write<Texture2DResource>("depthScreen");
}

//-------------------------------------------------------------------------------------------------

void SSAOPass::execute(const Camera* camera, const RenderOptions* renderOptions)
{
    const SSAOPassData& data = this->data();

    //std::vector<IFramebufferAttachable*> colorBuffers;
    //colorBuffers.push_back((GLTextureObject*)data.inputColour[0]->resource());
    ////colorBuffers.push_back((GLTextureObject*)data.outputColor[1]->resource());
    //getGLRenderManager()->getFBO()->attach(colorBuffers, (GLTextureObject*)data.outputDepth->resource(), true);
    ////getGLRenderManager()->getFBO()->bind();

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //getGLRenderManager()->getFBO()->blitToBuffer(0, 0);

    int32_t currentShaderId = -1;
    Shader* currentShader = nullptr;
    int32_t shaderId = m_renderManager->getShaderManager()->find(2)->id();

    // TODO: built this somewhere else, pass into render passes
    ShaderBuiltInData builtIn;
    builtIn.projMat = camera->projectionMatrix();
    builtIn.viewMat = camera->viewMatrix();
    builtIn.normalMat = glm::inverse(glm::transpose(camera->viewMatrix()));

    for (RenderCommand* rc : *getRenderManager()->getRenderQueue()) {
        //if (rc->shaderId() == 0)
        //    continue;
        if (!rc->isActive())
            continue;

        if (rc->renderable()->category() != Renderable::CATEGORY_SCREEN)
            continue;

        if (currentShaderId != shaderId) {
            if (currentShader != nullptr)
                currentShader->unbind();

            currentShader = m_renderManager->getShaderManager()->get(shaderId);
            currentShader->bind();
            currentShader->updateDataBlock("BuildIns", &builtIn, sizeof(builtIn));
            //currentShader->setUniform("viewportSize", viewport.width(), viewport.height()); // slow.
            currentShaderId = rc->shaderId();
        }

        GLTextureObject* normalMap = (GLTextureObject*) data.inputColour[0]->resource();
        GLTextureObject* depthMap = (GLTextureObject*) data.inputColour[1]->resource();

        glActiveTexture(GL_TEXTURE0);
        normalMap->bind();
        currentShader->setUniform("normalMap", 0);
        glActiveTexture(GL_TEXTURE1);
        depthMap->bind();
        currentShader->setUniform("depthMap", 1);
        glActiveTexture(GL_TEXTURE2);
        getRenderManager()->getTextureResource("random")->bind(); 
        currentShader->setUniform("randomMap", 2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        rc->execute(m_renderManager, currentShader);

        glActiveTexture(GL_TEXTURE2);
        getRenderManager()->getTextureResource("random")->unbind();
        glActiveTexture(GL_TEXTURE1);
        depthMap->unbind();
        glActiveTexture(GL_TEXTURE0);
        normalMap->unbind();
    }
}

//-------------------------------------------------------------------------------------------------

DKEViewerRenderManager* SSAOPass::getRenderManager()
{
    return dynamic_cast<DKEViewerRenderManager*>(m_renderManager);
}

//-------------------------------------------------------------------------------------------------
} // namespace dke
//-------------------------------------------------------------------------------------------------

