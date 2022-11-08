#include <graphics_gl/GLTextureObject.h>

//#include <core/image/CImgeLdr.h>

using namespace dke::graphics;

namespace dke {
namespace graphics_gl {

/*=============================================================================
   static functions
=============================================================================*/

GLTextureObject* GLTextureObject::createRGBA8(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

//GLTextureObject *GLTextureObject::createRGBA8(int32_t width, int32_t height, GLenum format, void *data)
//{
//    GLTextureObject *t = new GLTextureObject(GL_TEXTURE_2D, GL_RGBA8, format, GL_UNSIGNED_BYTE, width, height, data);
//    return t;
//}
//
//GLTextureObject *GLTextureObject::createRGB8(int32_t width, int32_t height, GLenum format, void *data)
//{
//    GLTextureObject *t = new GLTextureObject(GL_TEXTURE_2D, GL_RGB8, format, GL_UNSIGNED_BYTE, width, height, data);
//    return t;
//}

GLTextureObject* GLTextureObject::createRGBA16F(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createDEPTH24_STENCIL8(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createMultisampleRGBA8(int32_t width, int32_t height, int32_t numSamples, void* data)
{
    //GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height, numSamples, data);
    //return t;
    return nullptr;
}

GLTextureObject* GLTextureObject::createMultisampleRGBA16F(int32_t width, int32_t height, int32_t numSamples, void* data)
{
    //GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, GL_RGBA, GL_FLOAT, width, height, numSamples, data);
    //return t;
    return nullptr;
}

GLTextureObject* GLTextureObject::createFromRGB(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createFromBGR(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createFromRGBA(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createFromBGRA(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createFromALPHA(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

GLTextureObject* GLTextureObject::createFromLUMINANCE(int32_t width, int32_t height, void* data)
{
    GLTextureObject* t = new GLTextureObject(GL_TEXTURE_2D, GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, width, height, data);
    return t;
}

/*=============================================================================
   class functions
=============================================================================*/

GLTextureObject::GLTextureObject()
    : m_data(nullptr)
{
}

GLTextureObject::GLTextureObject(GLenum target, GLenum internalFormat, GLenum format, GLenum dataType, int32_t width, int32_t height, void* data)
    : m_target(target)
    , m_internalFormat(internalFormat)
    , m_format(format)
    , m_dataType(dataType)
    , m_width(width)
    , m_height(height)
    , m_data(data)
{
    create();
}

GLTextureObject::GLTextureObject(GLenum target, GLenum internalFormat, GLenum format, GLenum dataType, int32_t width, int32_t height, int32_t numSamples, void* data)
    : m_target(target)
    , m_internalFormat(internalFormat)
    , m_format(format)
    , m_dataType(dataType)
    , m_width(width)
    , m_height(height)
    , m_numSamples(numSamples)
    , m_data(data)
{
    create();
}

GLTextureObject::~GLTextureObject()
{
    destroy();
}

int32_t GLTextureObject::getId()
{
    return m_id;
}

int32_t GLTextureObject::create()
{
    glGenTextures(1, &m_id);
    bind();

    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (m_target == GL_TEXTURE_2D) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(m_target, 0, m_internalFormat, m_width, m_height, 0, m_format, m_dataType, m_data);
    }
    //else if (m_target == GL_TEXTURE_2D_MULTISAMPLE)
    //glTexImage2DMultisample(m_target, m_numSamples, m_internalFormat, m_width, m_height, true);

    if (m_internalFormat != GL_DEPTH24_STENCIL8) // should exclude all depth formats
        glGenerateMipmap(m_target);

    return 0;
}

void GLTextureObject::destroy()
{
    glDeleteTextures(1, &m_id);
}

void GLTextureObject::bind()
{
    glBindTexture(m_target, m_id);
}

void GLTextureObject::unbind()
{
    glBindTexture(m_target, 0);
}

bool GLTextureObject::resize(int32_t width, int32_t height)
{
    return false;
}

int32_t GLTextureObject::getTarget()
{
    return m_target;
}

void GLTextureObject::setTarget(int32_t target)
{
    m_target = (GLenum)target;
}

int32_t GLTextureObject::getFormat()
{
    return m_format;
}

void GLTextureObject::setFormat(int32_t format)
{
    m_format = (GLenum)format;
}

int32_t GLTextureObject::getInternalFormat()
{
    return m_internalFormat;
}

void GLTextureObject::setInternalFormat(int32_t internalFormat)
{
    m_internalFormat = (GLenum)internalFormat;
}

int32_t GLTextureObject::getWidth()
{
    return m_width;
}

void GLTextureObject::setWidth(int32_t width)
{
    m_width = width;
}

int32_t GLTextureObject::getHeight()
{
    return m_height;
}

void GLTextureObject::setHeight(int32_t height)
{
    m_height = height;
}

int32_t GLTextureObject::getWrap(int32_t wrapType)
{
    int32_t index = 0;
    switch (wrapType) {
    case GL_TEXTURE_WRAP_S:
        index = 0;
        break;
    case GL_TEXTURE_WRAP_T:
        index = 1;
        break;
    case GL_TEXTURE_WRAP_R:
        index = 2;
        break;
    default:
        break;
    }

    return m_wrap[index];
}

void GLTextureObject::setWrap(int32_t wrapType, int32_t param)
{
    int32_t index = 0;
    switch (wrapType) {
    case GL_TEXTURE_WRAP_S:
        index = 0;
        break;
    case GL_TEXTURE_WRAP_T:
        index = 1;
        break;
    case GL_TEXTURE_WRAP_R:
        index = 2;
        break;
    default:
        break;
    }

    m_wrap[index] = (GLenum)param;
}

int32_t GLTextureObject::getFilter(int32_t filterType)
{
    int32_t index = 0;
    switch (filterType) {
    case GL_TEXTURE_MAG_FILTER:
        index = 0;
        break;
    case GL_TEXTURE_MIN_FILTER:
        index = 1;
        break;
    default:
        break;
    }

    return m_filter[index];
}

void GLTextureObject::setFilter(FilterType filterType, FilterValue param)
{
    bind();
    glTexParameteri(m_target, (GLenum)filterType, (GLenum)param);
}

void GLTextureObject::setData(void* data)
{
    m_data = data;

    // TODO: bind and create texture again
}

int32_t GLTextureObject::getObjectId()
{
    return m_id;
}

int32_t GLTextureObject::getObjectType()
{
    return IFramebufferObject::FBO_ATTACHMENT_TEXTURE;
}

int32_t GLTextureObject::getAttachment()
{
    int32_t attachment;
    switch (m_internalFormat) {
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    //case GL_DEPTH_COMPONENT32:
    case GL_DEPTH_COMPONENT32F:
        attachment = GL_DEPTH_ATTACHMENT;
        break;
    case GL_DEPTH24_STENCIL8:
        attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        break;
    default:
        attachment = GL_COLOR_ATTACHMENT0;
        break;
    }
    return attachment;
}

int32_t GLTextureObject::getAttachmentTarget()
{
    return m_target;
}

//=============================================================================
//
//void GLTextureManager::loadPendingTextures()
//{
//    std::vector<std::string> completedKeys;
//    completedKeys.reserve(m_pendingTextureMap.size());
//
//    for (auto itr = m_pendingTextureMap.begin(); itr != m_pendingTextureMap.end(); ++itr) {
//#ifdef SUPPORT_IMAGES // until CImage API ready
//        CImgePtr apImg;
//        CImgeLdrPtr apImgeLdr = new CImgeLdr();
//        if (apImgeLdr->load(itr->second)) {
//            apImg = apImgeLdr->getImge(false, false, true);
//            if (apImg.valid()) {
//                // TODO: If we are going to use this code with ANGLE (Direct3D) we will need to worry about power-of-2 images.
//                CImgePtr apFullImg = CImgeLdr::assembleImage(apImg);
//                GLTextureObject* glTxt = nullptr;
//                uint32_t w = apFullImg->getWdth();
//                uint32_t h = apFullImg->getHght();
//                void* data = const_cast<void*>(apFullImg->getGLData());
//
//                switch (apFullImg->getPxlFrmt()) {
//                case CImge::pfRGBA:
//                    glTxt = GLTextureObject::createFromRGBA(w, h, data);
//                    break;
//                case CImge::pfRGB:
//                    glTxt = GLTextureObject::createFromRGB(w, h, data);
//                    break;
//                case CImge::pfBGRA:
//                    glTxt = GLTextureObject::createFromBGRA(w, h, data);
//                    break;
//                case CImge::pfBGR:
//                    glTxt = GLTextureObject::createFromBGR(w, h, data);
//                    break;
//                case CImge::pfNONE:
//                case CImge::pfALPHA:
//                case CImge::pfLUMINANCE_ALPHA:
//                case CImge::pfLUMINANCE:
//                    break;
//                }
//                if (glTxt != nullptr) {
//                    m_textureMap.insert(std::make_pair(itr->first, glTxt));
//                    completedKeys.emplace_back(itr->first);
//                }
//            }
//        }
//#endif // SUPPORT_IMAGES
//    }
//
//    // Delete completed entries from pending.
//    for (auto key : completedKeys)
//        m_pendingTextureMap.erase(key);
//}
//
//int32_t GLTextureManager::loadTexture(excore::CImgePtr imageData)
//{
//    CImgePtr apFullImg = CImgeLdr::assembleImage(imageData, false);
//    GLTextureObject* glTxt = nullptr;
//    uint32_t w = apFullImg->getWdth();
//    uint32_t h = apFullImg->getHght();
//    void* data = const_cast<void*>(apFullImg->getGLData());
//    int32_t id = -1;
//
//    switch (apFullImg->getPxlFrmt()) {
//    case CImge::pfRGBA:
//        glTxt = GLTextureObject::createFromRGBA(w, h, data);
//        break;
//    case CImge::pfRGB:
//        glTxt = GLTextureObject::createFromRGB(w, h, data);
//        break;
//    case CImge::pfBGRA:
//        apFullImg->swizzle();
//        glTxt = GLTextureObject::createFromBGRA(w, h, data);
//        break;
//    case CImge::pfBGR:
//        apFullImg->swizzle();
//        glTxt = GLTextureObject::createFromBGR(w, h, data);
//        break;
//    case CImge::pfLUMINANCE:
//        glTxt = GLTextureObject::createFromLUMINANCE(w, h, data);
//        break;
//    case CImge::pfALPHA:
//        glTxt = GLTextureObject::createFromALPHA(w, h, data);
//        break;
//    case CImge::pfLUMINANCE_ALPHA:
//        // TODO: Not supported.
//        assert(false);
//    default:
//        glTxt = nullptr;
//    }
//
//    if (glTxt != nullptr) {
//        id = glTxt->getId();
//        m_textureIdMap.insert(std::make_pair(id, glTxt));
//    }
//
//    return id;
//}
//
//int32_t GLTextureManager::loadTexture(HEXImage hImage) 
//{
//    GLTextureObject* glTxt = nullptr;
//    uint32_t w = ex_imageWidth(hImage);
//    uint32_t h = ex_imageHeight(hImage);
//    void* data = const_cast<void*>((void*)ex_imageData(hImage));
//    int32_t id = -1;
//
//    switch (ex_imageType(hImage)) {
//    case kEXImagePAM:
//        glTxt = GLTextureObject::createFromRGBA(w, h, data);
//        break;
//    case kEXImagePPM:
//        glTxt = GLTextureObject::createFromRGB(w, h, data);
//        break;
//    case kEXImagePGM:
//        glTxt = GLTextureObject::createFromLUMINANCE(w, h, data);
//        break;
//    case kEXImagePBM: {
//        //I don't think opengl likes 1bpp textures
//        HEXImage hTmpImage = nullptr;
//        ex_imageCreatePGM(w,h,255,&hTmpImage);
//        ex_imageCopyData(hTmpImage, hImage, 0, 0, w, h);
//
//        data = const_cast<void*>((void*)ex_imageData(hTmpImage));
//        glTxt = GLTextureObject::createFromLUMINANCE(w, h, data);
//        ex_imageFree(hTmpImage);
//        break;
//    }    
//    case kEXImageUnknown:
//        // TODO: Not supported.
//        assert(false);
//    default:
//        glTxt = nullptr;
//    }
//
//    if (glTxt != nullptr) {
//        id = glTxt->getId();
//        m_textureIdMap.insert(std::make_pair(id, glTxt));
//    }
//
//    return id;
//}

}
}
