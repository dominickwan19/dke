#include <graphics_gl/GLShader.h>
#include <GL/glew.h>
#include <algorithm>
#include <vector>

//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

void GLShader::bind()
{
    glUseProgram(m_id);

    // If there's any uniform blocks used, bind them as well.
    for (auto p : m_uniformDataBindings) {
        if (p.second.name == "BuildIns")
            glBindBufferRange(GL_UNIFORM_BUFFER, p.second.blockIndex, p.second.dataBufferHandle, 0, p.second.blockSize);
    }
}

//-------------------------------------------------------------------------------------------------

void GLShader::unbind()
{
    glUseProgram(0);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, void* val)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform2fv(loc, 1, (const GLfloat*)val);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, int32_t val)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform1i(loc, (GLint)val);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, float v0)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform1f(loc, (GLfloat)v0);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, float v0, float v1)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform2f(loc, (GLfloat)v0, (GLfloat)v1);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, float v0, float v1, float v2)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform3f(loc, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniform(const char* uniformName, float v0, float v1, float v2, float v3)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniform4f(loc, (GLfloat)v0, (GLfloat)v1, (GLfloat)v2, (GLfloat)v3);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setUniformMatrix4(const char* uniformName, void* val)
{
    if (!uniformName)
        return;

    GLint loc = glGetUniformLocation(m_id, uniformName);
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)val);
}

//-------------------------------------------------------------------------------------------------

void GLShader::setDataBinding(uint32_t dataBlockIndex, const std::string& dataBlockName, uint32_t dataBufferHandle, uint32_t blockSize)
{
    m_uniformDataBindings.insert(std::make_pair(dataBlockIndex, DataBinding(dataBlockIndex, dataBlockName, dataBufferHandle, blockSize)));
}

//-------------------------------------------------------------------------------------------------

void GLShader::updateDataBlock(const std::string& dataBlockName, void* val, size_t dataSize)
{
    // bad find_if here. could avoid by restructuring the map
    auto it = std::find_if(m_uniformDataBindings.begin(), m_uniformDataBindings.end(),
        [&](const std::pair<int32_t, DataBinding>& p) -> bool { return p.second.name == dataBlockName; });
    if (it == m_uniformDataBindings.end())
        return;

    glBindBuffer(GL_UNIFORM_BUFFER, it->second.dataBufferHandle);
    //GLvoid* p = glMapBufferOES(GL_UNIFORM_BUFFER, GL_WRITE_ONLY_OES);
    //memcpy(p, val, dataSize);
    glBufferData(GL_UNIFORM_BUFFER, dataSize, val, GL_DYNAMIC_DRAW);
    //glUnmapBuffer(GL_UNIFORM_BUFFER);
}

//-------------------------------------------------------------------------------------------------

} // namespace graphics_gl
} // namespace dke