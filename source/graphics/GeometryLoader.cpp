//-------------------------------------------------------------------------------------------------
#include <graphics/GeometryLoader.h>
//-------------------------------------------------------------------------------------------------
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <graphics/Geometry.h>
#include <graphics/geometry/MeshGeometry.h>
#include <gl/glew.h>
#include <memory>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

GeometryLoader::GeometryLoader()
{
}

GeometryLoader::~GeometryLoader()
{
}



template <>
void GeometryLoader::loadFromFile(std::vector<MeshGeometry*>& geometries, const std::string& filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
        return;

    auto processMesh = [&](aiMesh* mesh, const aiScene* scene) {
        MeshGeometry* m = new MeshGeometry;
        m->addPosition((float*)mesh->mVertices, mesh->mNumVertices * 3);
        m->addNormal((float*)mesh->mNormals, mesh->mNumVertices * 3);
        std::vector<float> colour(mesh->mNumVertices * 4);
        for (int i = 0; i < colour.size(); i++)
            colour[i] = 1.0f;
        m->addColour(colour.data(), colour.size());

        std::vector<int> indices;
        for (int i = 0; i < mesh->mNumFaces; i++) {
            for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }
        m->addIndices(indices.data(), indices.size());

        geometries.push_back(m);
    };

    std::function<void(aiNode*, const aiScene*)> processNode = [&](aiNode* node, const aiScene* scene) {
        for (GLuint i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (GLuint i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    };

    processNode(scene->mRootNode, scene);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
