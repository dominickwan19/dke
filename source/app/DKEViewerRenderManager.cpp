//-------------------------------------------------------------------------------------------------
#include <app/DKEViewerRenderManager.h>
//-------------------------------------------------------------------------------------------------
#include <GL/glew.h>
#include <graphics/Geometry/ScreenQuadGeometry.h>
#include <graphics/GeometryLoader.h>
#include <graphics/Renderable.h>
#include <graphics/geometry/MeshGeometry.h>
#include <graphics_gl/GLRenderManager.h>
#include <graphics_gl/GLRenderable.h>
#include <graphics_gl/GeometryPass.h>
#include <graphics_gl/GLTextureObject.h>
#include <stdio.h>
#include <random>
//-------------------------------------------------------------------------------------------------
#include <app/SSAOPass.h>
//-------------------------------------------------------------------------------------------------
using namespace dke::graphics;
using namespace dke::graphics_gl;
//-------------------------------------------------------------------------------------------------
namespace dke {
//-------------------------------------------------------------------------------------------------

DKEViewerRenderManager::DKEViewerRenderManager()
{
}

//-------------------------------------------------------------------------------------------------

DKEViewerRenderManager::~DKEViewerRenderManager()
{
}

//-------------------------------------------------------------------------------------------------

void DKEViewerRenderManager::onInitialize()
{
    renderOptions()->backgroundColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    std::string shaderBase = "shader/";
    int32_t shaderId = 0;
    int32_t shaderId2 = 0;
    int32_t shaderId3 = 0;
    int32_t shaderId4 = 0;
    shaderId = m_shaderManager.create({ shaderBase + "screenHighlight.vert.glsl", shaderBase + "screenHighlight.frag.glsl" }, 0);
    shaderId2 = m_shaderManager.create({ shaderBase + "geometry.vert.glsl", shaderBase + "geometry.frag.glsl" }, 1);
    shaderId3 = m_shaderManager.create({ shaderBase + "ssao.vert.glsl", shaderBase + "ssao.frag.glsl" }, 2);
    //shaderId4 = m_shaderManager.create({ shaderBase + "ssao.vert.glsl", shaderBase + "hbao.frag.glsl" }, 3);

    //loadMesh();
    GeometryLoader loader;
    MeshGeometry* mesh = nullptr;
    std::vector<MeshGeometry*> geometries;
    loader.loadFromFile(geometries, "bunny.obj");
    Renderable* renderable = new GLRenderable();
    for (auto g : geometries)
        renderable->addGeometry(g);
    renderable->setCategory(Renderable::CATEGORY_GEOMETRY);
    renderable->setMaterial(shaderId2);
    addRenderable(renderable);

    initRenderPasses();
    createRandomTexture();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerRenderManager::initRenderPasses()
{
    // create a screen quad for rendering screen-based passes
    int shaderId = m_shaderManager.find(0)->id();
    Renderable* renderable = new GLRenderable();
    ScreenQuadGeometry* screenQuad = new ScreenQuadGeometry;
    float quad[12] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
    screenQuad->addPosition(quad, 12);
    renderable->setCategory(Renderable::CATEGORY_SCREEN);
    renderable->addGeometry(screenQuad);
    renderable->setMaterial(shaderId);
    addRenderable(renderable);

    Framegraph& fg = framegraph();

    // texture formats in framegraph
    auto tdType0 = TextureDescription(0);
    auto tdType1 = TextureDescription(1);

    // TODO: probably can automate this create process with specifications in render passes
    fg.createResourceHandle<Texture2DResource>("normalMap", 0, tdType0);
    fg.createResourceHandle<Texture2DResource>("materialMap", 0, tdType0);
    fg.createResourceHandle<Texture2DResource>("surfaceColorMap", 0, tdType0);
    fg.createResourceHandle<Texture2DResource>("depth", 1, tdType1);
    fg.createResourceHandle<Texture2DResource>("ssaoMap", 0, tdType0);
    fg.createResourceHandle<Texture2DResource>("depthScreen", 1, tdType1); // TODO: see if we can bind no depth target to FBO

    // initialize render passes
    addRenderPass(new GeometryPass(0, this));
    auto ssaoPass = new SSAOPass(1, this, renderable);
    addRenderPass(ssaoPass);

    compileFramegraph();
}

//-------------------------------------------------------------------------------------------------

void DKEViewerRenderManager::createRandomTexture()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    std::vector<int8_t> randomData;
    randomData.resize(16 * 16 * 4);
    for (int i = 0; i < 16 * 16 * 4; i++) {
        randomData[i] = dist(mt);
    }

    m_randomTexture = GLTextureObject::createRGBA8(16, 16, randomData.data());
    m_textureResourceMap.emplace("random", m_randomTexture);
}

//-------------------------------------------------------------------------------------------------

ITextureObject* DKEViewerRenderManager::getTextureResource(const std::string&& resourceName)
{
    auto it = m_textureResourceMap.find(resourceName);
    if (it != m_textureResourceMap.end())
        return it->second.get();

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------------------