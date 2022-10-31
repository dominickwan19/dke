//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderCommand_h
#define DKE_Graphics_RenderCommand_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Geometry.h>
#include <graphics/Colour.h>
#include <string>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------
class Geometry;
class GeometryBuffer;
class Renderable;
class RenderManager;
class RenderCommand;
class RenderOptions;
class Shader;
//-------------------------------------------------------------------------------------------------

enum RenderCommandType {
    RC_DRAW_ARRAYS,
    RC_DRAW_ELEMENTS,
    RC_MULTI_DRAW_ELEMENTS
};

//-------------------------------------------------------------------------------------------------

class DrawData {
public:
    explicit DrawData(uint32_t id)
        : m_typeId(id)
    {
    }
    virtual ~DrawData() {}

    uint32_t typeId() const { return m_typeId; }

protected:
    DrawData() = delete;

private:
    uint32_t m_typeId;
};

//-------------------------------------------------------------------------------------------------

class DrawArraysData : public DrawData {
public:
    DrawArraysData()
        : DrawData(RC_DRAW_ARRAYS)
        , geometryType(0)
        , first(0)
        , dataCount(0)
    {
    }

    uint32_t geometryType;
    uint32_t first;
    uint32_t dataCount;

    // NOTE: Should be moved to material impl later
    std::string textureName;
};

//-------------------------------------------------------------------------------------------------

class DrawElementsData : public DrawData {
public:
    DrawElementsData()
        : DrawData(RC_DRAW_ELEMENTS)
        , geometryType(0)
        , dataCount(0)
        , baseVertex(0)
        , indexStart(0)
    {
    }

    uint32_t geometryType;
    uint32_t dataCount;
    uint32_t baseVertex;
    uint32_t indexStart; // for VBO/VAO. Needs to handle indices array that are passed to DrawElements directly
};

//-------------------------------------------------------------------------------------------------

class MultiDrawElementsData : public DrawData {
public:
    MultiDrawElementsData()
        : DrawData(RC_MULTI_DRAW_ELEMENTS)
        , geometryType(0)
        , dataCount(0)
        , indices(nullptr)
        , indexStarts(nullptr)
        , baseVertices(nullptr)
    {
    }

    uint32_t geometryType;
    int32_t dataCount;
    int32_t* indices;
    uintptr_t* indexStarts;
    int32_t* baseVertices;
};

//-------------------------------------------------------------------------------------------------

class RenderCommandGenerator {
public:
    virtual std::vector<RenderCommand*> generate() const = 0;
    virtual std::vector<RenderCommand*> generate(Renderable* renderable) const = 0;
};

//-------------------------------------------------------------------------------------------------

class RenderCommand {
public:
    RenderCommand();
    virtual ~RenderCommand() {}

    uint32_t id() const { return m_id; }
    uint64_t key() const { return m_key; }
    void setKey(uint64_t key) { m_key = key; }

    virtual void execute(const RenderManager* renderManager, Shader* shader, RenderOptions* renderOptions = nullptr);

    const Geometry::GeometryBufferHandle& geometryHandle() const;
    void setGeometryHandle(const Geometry::GeometryBufferHandle& h);

    void setActive(bool active);
    bool isActive() const;
    void setHighlightColour(const Colour4& colour);
    const Colour4& highlightColour() const;
    void setIsHighlighted(bool h);
    bool isHighlighted() const;

    void setUseBlending(bool b);
    bool useBlending() const;
    void setUseDepthTest(bool d);
    bool useDepthTest() const;
    void setUseStencil(bool s);
    bool useStencil() const;

    int32_t geometryBufferId() const { return m_geometryBufferId; }
    void setGeometryBufferId(int32_t geometryBufferId) { m_geometryBufferId = geometryBufferId; }
    int32_t shaderId() const { return m_shaderId; }
    void setShaderId(int32_t shaderId) { m_shaderId = shaderId; }
    int32_t textureId() const { return m_textureId; }
    void setTextureId(int32_t textureId) { m_textureId = textureId; }

    const Geometry* geometry() const { return m_geometry; }
    void setGeometry(Geometry* geometry) { m_geometry = geometry; }

    const GeometryBuffer* geometryBuffer() const { return m_geometryBuffer; }
    void setGeometryBuffer(GeometryBuffer* geometryBuffer) { m_geometryBuffer = geometryBuffer; }

    const Renderable* renderable() const { return m_renderable; }
    void setRenderable(Renderable* renderable) { m_renderable = renderable; }

    const DrawData* drawData() const { return m_drawData.get(); }

    // This takes ownership of the drawing data.
    void setDrawData(DrawData* pDrawData);

protected:
    // Allow non-const access in derived classes.
    GeometryBuffer* m_geometryBuffer;
    Renderable* m_renderable;

private:
    uint32_t m_id;
    uint64_t m_key;
    Geometry* m_geometry;

    // draw data
    int32_t m_geometryBufferId;
    int32_t m_shaderId;
    int32_t m_textureId; 

    std::unique_ptr<DrawData> m_drawData;

    Colour4 m_highlightColour;
    bool m_bActive;
    bool m_bIsHighlighted;

    // Dominic: maybe combine into 1 int8_t bit flag?
    bool m_useBlending;
    bool m_useDepthTest;
    bool m_useStencil;

private:
    Geometry::GeometryBufferHandle m_geometryHandle;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderCommand_h
