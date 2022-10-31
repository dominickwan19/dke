#ifndef GLTextureObject_h_
#define GLTextureObject_h_

#include <GL/glew.h>
#include <graphics/IFramebufferObject.h>
#include <graphics/ITextureObject.h>

#include <graphics/MaterialManager.h>

//TODO: use doxygen style comment

class GLTextureObject : public dke::graphics::ITextureObject, public dke::graphics::IFramebufferAttachable {
public:
    // helper functions for creating texture object
    // these are the recommended functions for creating a texture - they will take
    // care of some internal flags like dataType and format
    static GLTextureObject* createRGBA8(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createRGBA16F(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createDEPTH24_STENCIL8(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createMultisampleRGBA8(int32_t width, int32_t height, int32_t numSamples, void* data = nullptr);
    static GLTextureObject* createMultisampleRGBA16F(int32_t width, int32_t height, int32_t numSamples, void* data = nullptr);

    static GLTextureObject* createFromRGB(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createFromBGR(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createFromRGBA(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createFromBGRA(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createFromALPHA(int32_t width, int32_t height, void* data = nullptr);
    static GLTextureObject* createFromLUMINANCE(int32_t width, int32_t height, void* data = nullptr);

public:
    GLTextureObject();
    GLTextureObject(GLenum target, GLenum internalFormat, GLenum format, GLenum dataType, int32_t width, int32_t height, void* data);
    GLTextureObject(GLenum target, GLenum internalFormat, GLenum format, GLenum dataType, int32_t width, int32_t height, int32_t numSamples, void* data);
    ~GLTextureObject();

    // functions from ITextureObject
    virtual int32_t getId() override;

    virtual int32_t create() override;
    virtual void destroy() override;

    virtual void bind() override;
    virtual void unbind() override;

    virtual int32_t getTarget() override;
    virtual void setTarget(int32_t target) override;

    virtual int32_t getFormat() override;
    virtual void setFormat(int32_t format) override;

    virtual int32_t getInternalFormat() override;
    virtual void setInternalFormat(int32_t internalFormat) override;

    virtual int32_t getWidth() override;
    virtual void setWidth(int32_t width) override;

    virtual int32_t getHeight() override;
    virtual void setHeight(int32_t height) override;

    virtual int32_t getWrap(int32_t wrapType) override;
    virtual void setWrap(int32_t wrapType, int32_t param) override;

    virtual int32_t getFilter(int32_t filterType) override;
    virtual void setFilter(dke::graphics::FilterType filterType, dke::graphics::FilterValue param) override;

    virtual void setData(void* data) override;

    // functions from IFramebufferAttachable
    virtual int32_t getObjectId() override;
    virtual int32_t getObjectType() override;
    virtual int32_t getAttachment() override;
    virtual int32_t getAttachmentTarget() override;

    virtual bool resize(int32_t width, int32_t height) override;

private:
    GLuint m_id;
    int32_t m_width;
    int32_t m_height;
    int32_t m_numSamples; // multisample only
    GLenum m_target;
    GLenum m_format;
    GLenum m_internalFormat;
    GLenum m_dataType;
    GLenum m_wrap[4];
    GLenum m_filter[2];
    void* m_data;
};

//class GLTextureManager : public graphics::TextureManager {
//public:
//    virtual void loadPendingTextures() override;
//    virtual int32_t loadTexture(excore::CImgePtr imageData) override;
//    virtual int32_t loadTexture(HEXImage hImage) override;
//};

#endif // GLTextureObject_h_
