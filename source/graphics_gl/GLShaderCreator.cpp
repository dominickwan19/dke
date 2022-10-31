#include <graphics_gl/GLShaderCreator.h>
//-------------------------------------------------------------------------------------------------
#include <graphics_gl/GLShader.h>
#include <GL/glew.h>
#include <algorithm>
#include <fstream>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics_gl {
//-------------------------------------------------------------------------------------------------

std::string read_file(const char* filename)
{
    std::string file_str;
    std::ifstream is(filename, std::ifstream::in);
    //printf("loading shader: %s\n", filename);

    if (is) {
        is.seekg(0, is.end);
        unsigned int length = (unsigned int)is.tellg();
        is.seekg(0, is.beg);

        file_str.resize(length);

        is.read(&file_str[0], length);
        is.close();
    }
    return file_str;
}

//-------------------------------------------------------------------------------------------------

GLuint load_shader(const char* filename, GLenum shader_type)
{
    std::string shader_source = read_file(filename);

    GLuint shader_handle = glCreateShader(shader_type);
    const GLchar* source = (const GLchar*)shader_source.c_str();
    glShaderSource(shader_handle, 1, &source, 0);

    glCompileShader(shader_handle);

    GLint is_compiled = 0;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &is_compiled);
    if (!is_compiled) {
        GLint max_length = 0;
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader_handle, max_length, &max_length, &info_log[0]);
        printf("Error compiling shader %s(%d):\n%s\n", filename, shader_type, info_log.data());
        glDeleteShader(shader_handle);

        // print the info log

        return -1;
    }

    return shader_handle;
}

//-------------------------------------------------------------------------------------------------

GLuint createShaders(std::string vertShaderSrc, std::string fragShaderSrc)
{
    GLuint vertex_shader_handle = load_shader(vertShaderSrc.c_str(), GL_VERTEX_SHADER);
    if (vertex_shader_handle == -1)
        return 0;

    GLuint fragment_shader_handle = load_shader(fragShaderSrc.c_str(), GL_FRAGMENT_SHADER);
    if (fragment_shader_handle == -1)
        return 0;

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(program, vertex_shader_handle);
    glAttachShader(program, fragment_shader_handle);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        printf("Error compiling shading program %d:\n%s\n", program, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(program);
        // Don't leak shaders either.
        glDeleteShader(vertex_shader_handle);
        glDeleteShader(fragment_shader_handle);

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        return 0;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertex_shader_handle);
    glDetachShader(program, fragment_shader_handle);

    return program;
}

//-------------------------------------------------------------------------------------------------

graphics::Shader* GLShaderCreator::createShader(int8_t shaderStageFlags, std::vector<std::string> shaderSources)
{
    graphics::Shader* shader = new GLShader();

    // this doesn't handle shaders in other stage properly - will need to check shaderstageFlags and create shader according to that
    // e.g. compute shader only takes one source, normal shader takes 2 (vs + fs), and could be expanded to 4 (vs + ts + gs + fs)
    GLuint id = createShaders(shaderSources[0], shaderSources[1]);
    shader->setId(id);

    getUniforms(shader);
    getUniformBlocks(shader);
    return shader;
}

//-------------------------------------------------------------------------------------------------

void GLShaderCreator::getUniforms(graphics::Shader*& shader)
{
    // https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_query

    GLint numUniforms = 0;
    glGetProgramiv(shader->id(), GL_ACTIVE_UNIFORMS, &numUniforms);

    GLint maxUniformNameLength = 0;
    glGetProgramiv(shader->id(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    std::vector<char> name(maxUniformNameLength, 0);
    //for (auto i = 0; i < numUniforms; i++) {
        //GLsizei actualLength;
        //glGetActiveUniformName(shader->id(), i, maxUniformNameLength, &actualLength, name);
    //}

    // generate uniform map: name -> uniform location
}

//-------------------------------------------------------------------------------------------------

void GLShaderCreator::getUniformBlocks(graphics::Shader*& shader)
{
    GLint numUniformBlocks = 0;
    glGetProgramiv(shader->id(), GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

    GLint maxUniformBlockNameLength = 0;
    glGetProgramiv(shader->id(), GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxUniformBlockNameLength);

    std::vector<char> name(maxUniformBlockNameLength, 0);
    for (auto i = 0; i < numUniformBlocks; i++) {
        GLsizei actualLength;
        glGetActiveUniformBlockName(shader->id(), i, maxUniformBlockNameLength, &actualLength, name.data());
        //GLenum err = glGetError();

        GLsizei blockSize = 0;
        glGetActiveUniformBlockiv(shader->id(), i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

        //#error not handling shared buffer here
        //#error for instance we are sharing BuildIns accroesss diffe

        GLuint uboHandle = 0;
        glGenBuffers(1, &uboHandle);
        glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
        glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_STATIC_READ);

        int blockIndex = glGetUniformBlockIndex(shader->id(), name.data());
        glUniformBlockBinding(shader->id(), blockIndex, i);
        glBindBufferBase(GL_UNIFORM_BUFFER, i, uboHandle);
        //glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboHandle, 0, blockSize);

        // map: block name, block index/binding point, ubo buffer id
        shader->setDataBinding(blockIndex, name.data(), uboHandle, blockSize);
        shader->setDataBlockInfo(blockIndex, name.data(), blockSize, i);
    }
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics_gl
} // namespace dke
//-------------------------------------------------------------------------------------------------
