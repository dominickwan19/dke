//-------------------------------------------------------------------------------------------------
#include <graphics/geometry/ScreenQuadGeometryBuffer.h>
//-------------------------------------------------------------------------------------------------
#include <GL/glew.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

ScreenQuadGeometryBuffer::ScreenQuadGeometryBuffer(const GeometryFormatDescription& gfd, uint32_t flags, size_t defaultSize)
    : GeometryBuffer(GL_TRIANGLES, flags, gfd, defaultSize)
    , m_vaoId(0)
{
}

//-------------------------------------------------------------------------------------------------

void ScreenQuadGeometryBuffer::cache()
{
    glGenVertexArrays(1, (GLuint*)&m_vaoId);
    glBindVertexArray(m_vaoId);

    int index = 0;
    for (const auto& b : m_buffers) {
        b->cacheToDevice();
        b->bind(0, 0);
        printf("index: %d, component size: %d\n", index, m_formatDescription.componentSizes[index]);
        glVertexAttribPointer(index, m_formatDescription.componentSizes[index], GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);

        index++;
    }

    glBindVertexArray(0);
}

//-------------------------------------------------------------------------------------------------

bool ScreenQuadGeometryBuffer::bind(Geometry::GeometryBufferHandle* handle)
{
    glBindVertexArray(m_vaoId);
    return true;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
