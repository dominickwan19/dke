//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Renderable_h
#define DKE_Graphics_Renderable_h
//-------------------------------------------------------------------------------------------------
#include <graphics/DynamicObj.h>
#include <graphics/Geometry.h>
#include <graphics/IRenderParameters.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class RenderCommand;
class RenderManager;
class RenderableKeyGenerator;
class Shader;

enum CategoryType {
    CATEGORY_WORLD = 0,
    CATEGORY_FULL_SCREEN,
    CATEGORY_HUD,
    CATEGORY_OVERLAY,
    CATEGORY_APPLICATION
};

enum ComponentType {
    COMPONENT_OBJECT,
    COMPONENT_ANNOTATION
};

enum EntityType {
    ENTITY_NONE = 0,
    ENTITY_BACKGROUND = 1,
    ENTITY_OBJECT,
    ENTITY_OBJECT_OUTLINE,
    IGHT,
    ENTITY_ANNOTATION,
    ENTITY_EFFECT,
    ENTITY_HUD,
    ENTITY_ALL
};

enum TranslucencyType {
    TRANSLUCENCY_OPAQUE = 0,
    TRANSLUCENCY_NORMAL,
    TRANSLUCENCY_ADDITIVE,
    TRANSLUCENCY_SUBTRACTIVE
};

/**
  Renderable is a piece of object to be rendered along with its states/parameters.

  Key of the renderable is an identity across the rendering system. The key will
  be used for bucketing and will also be used to determine their order of rendering
  during run time.

  It can contain multiple geometries each with their own material. Render commands
  are generated from the renderable, where different renderables can generate
  commands differently.
 */
class Renderable : public DynamicObj {
public:
    enum CategoryType {

    };

public:
    struct Attributes {
        int8_t sceneLayer;
        int8_t component;
        int8_t entity;
        int8_t viewportId;
        int8_t translucency;
        int8_t states;
        int32_t depth;
        int32_t material;
    };

public:
    Renderable();
    virtual ~Renderable();

    int32_t id() const { return m_id; }
    virtual uint32_t globalId() const { return m_id; }

    void setKey(uint64_t key);

    // Generate a new key for this Renderable based on current state of it.
    // The key should be able to encode attributes of this Renderable such
    // that it can be used for binning effectively in the render pipeline.
    virtual uint64_t getKey();

    CategoryType category() const;
    ComponentType component() const;
    EntityType entityType() const;
    TranslucencyType translucency() const;

    void setViewportId(int8_t);
    int8_t viewportId() const;// to subclass

    int8_t states() const;
    int32_t order() const;
    int32_t material() const;

    virtual void setKeyStruct(CategoryType category, ComponentType component, EntityType entityType, TranslucencyType translucency);

    void setDepthId(int32_t depthId);
    void setMaterial(int32_t material);

    void addGeometry(Geometry* geometry);
    std::vector<Geometry*>& geometries() { return m_geometries; }
    const std::vector<Geometry*>& geometries() const { return m_geometries; }
    void clearGeometryData();

    size_t getNumRenderCommands() const { return m_renderCommands.size(); }
    RenderCommand** getRenderCommands() { return m_renderCommands.data(); }
    virtual void setRenderCommands(std::vector<RenderCommand*>& renderCommands);
    virtual void addRenderCommand(RenderCommand* rc);
    void clearRenderCommands();

    size_t numGeometryHandles() const;
    void setGeometryHandle(int index, const Geometry::GeometryBufferHandle& h);
    const Geometry::GeometryBufferHandle& getGeometryHandle(int index) const;
    void invalidateGeometryHandles();

    bool hasTexture() const { return false; }
    virtual uint8_t transparencyFlag() const { return 0; }
    bool useBlending() const { return transparencyFlag() != 0; }

    bool visible() const { return m_visible; }
    void setVisible(bool v);

    virtual void updateRenderCommands(RenderableKeyGenerator* keyGen);

    // Asking users to call update makes the interface
    // hard to understand what to do for this function.
    // Target is to hide it in base class, only use
    // the function in underlying core process, and only ask
    // user for data (onRenderParameterUpdate/onGeometryUpdate)
    // when necessary.
    virtual void update() { }

    // The function to tell if the renderable is updated. 
    virtual bool isUpdated(uint64_t refTimestamp);

    // The renderable will have less thing to control.
    // Not sure if we need to keep this function.
    virtual void destroy() { }

    /*========= Geometry related function =========================================*/

    // Construct geometry for this renderable.
    // The class does not take ownership of object created.
    // TODO: target is to replace createGeometry and cache functions in the class.
    virtual void onGeometryUpdate(std::vector<std::unique_ptr<Geometry>>& geometries) {  }

    // TODO: change name of this function to cacheGeometries.
    virtual void cache() { }

    /*========= Render Parameters related function ================================*/

    // Construct render parameters for this renderable.
    // The class does not take ownership of object created.
    virtual std::unique_ptr<IRenderParameters> onRenderParametersUpdate() { return nullptr; }

    // This should be in the base class. Sends render parameters
    // to buffer (and then GPU).
    virtual void cacheRenderParameters() { }

    // hurts performance if binding individual uniform in the function.
    // but still good for binding buffers or textures, and good
    // for quick implementation.
    virtual void onBindParameters(RenderCommand* rc, Shader* shader) { }

protected:
    Attributes m_attributes;

    /*!
     * \brief   A 64-bit key containing various information encoded, see Geometry Key Description.
     *          One renderable = one key, while the same scene entity can create multiple renderables.
     */
    uint64_t m_key;
    std::vector<Geometry*> m_geometries; //<! Geometry associated with this renderable.
    std::vector<Geometry::GeometryBufferHandle> m_geometryHandles;
    std::vector<RenderCommand*> m_renderCommands; // TODO: Don't think this class is the best place for storing render commands but let's just place it here first.

    bool m_visible;

    int32_t m_id;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Renderable_h
