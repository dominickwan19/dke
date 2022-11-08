//-------------------------------------------------------------------------------------------------
#include <graphics/geometry/MeshGeometryBuffer.h>
//-------------------------------------------------------------------------------------------------
#include <gl/glew.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

MeshGeometryBuffer::MeshGeometryBuffer(const GeometryFormatDescription& gfd, uint32_t flags, size_t defaultSize)
    : GeometryBuffer(GL_TRIANGLES, flags, gfd, defaultSize)
    , m_vaoId(0)
{
}

//-------------------------------------------------------------------------------------------------

void MeshGeometryBuffer::cache()
{
    GLuint vaoId;
    glGenVertexArrays(1, &vaoId);
    m_vaoId = vaoId;
    glBindVertexArray(m_vaoId);

    int index = 0;
    for (const auto& b : m_buffers) {
        b->cacheToDevice();
        b->bind(0, 0);
        glVertexAttribPointer(index, m_formatDescription.componentSizes[index], GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);

        index++;
    }

    glBindVertexArray(0);

    getIndexBuffer()->cacheToDevice();
}

//-------------------------------------------------------------------------------------------------

bool MeshGeometryBuffer::bind(Geometry::GeometryBufferHandle* handle)
{
    glBindVertexArray(m_vaoId);
    getIndexBuffer()->bind(0, 0);
    return true;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // nmaespace dke
//-------------------------------------------------------------------------------------------------
