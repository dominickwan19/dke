//-------------------------------------------------------------------------------------------------
#include <app/DKEViewerRenderManager.h>
//-------------------------------------------------------------------------------------------------
#include <GL/glew.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics/Renderable.h>
#include <graphics_gl/GLRenderable.h>
#include <graphics_gl/ScreenQuadGeometry.h>
#include <stdio.h>
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
using namespace dke::graphics_gl;
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

DKEViewerRenderManager::DKEViewerRenderManager()
{
}

DKEViewerRenderManager::~DKEViewerRenderManager()
{
}


void DKEViewerRenderManager::onInitialize() 
{
    renderOptions()->backgroundColour = glm::vec4(0.2f, 0.2f, 0.2f, 0.2f);

    std::string shaderBase = "shader/";
    int32_t shaderId = 0;
    shaderId = m_shaderManager.create({ shaderBase + "screenHighlight.vert.glsl", shaderBase + "screenHighlight.frag.glsl" }, 0);

    Renderable* renderable = new GLRenderable();
    ScreenQuadGeometry* screenQuad = new ScreenQuadGeometry;
    float quad[12] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
    screenQuad->addPosition(quad, 12);
    renderable->addGeometry(screenQuad);
    renderable->setMaterial(shaderId);
    addRenderable(renderable);
}

//-------------------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------------------