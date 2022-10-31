//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_StateManager_h_
#define DKE_Graphics_StateManager_h_
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <memory>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

enum RenderPipelineStates {
    RPS_DEPTH_TEST = 1,
    RPS_ALPHA_TEST = 2,
    RPS_BLENDING = 4,
    RPS_STENCIL_TEST = 8,
    RPS_CULL_FACE = 16,
};

enum TransparencyModes {
    TRANSPARENCY_NONE = 0,
    TRANSPARENCY_NORMAL, // srcColor * a + dstColor * (1 - a)
    TRANSPARENCY_ADD,
    TRANSPARENCY_SUBTRACT
};

enum TransparencyFactors {
    TF_ZERO,
    TF_ONE,
    TF_SRC_ALPHA,
    TF_ONE_MINUS_SRC_ALPHA
};

struct GraphicState {
    GraphicState() 
        : states(0)
        , transparencyMode(0)
        , srcFactor(0)
        , dstFactor(0)
    {

    }

    int16_t states;
    int16_t transparencyMode;
    int16_t srcFactor;
    int16_t dstFactor;
};

//-------------------------------------------------------------------------------------------------

class GraphicStateFuncs {
public:
    GraphicStateFuncs() = default;
    ~GraphicStateFuncs() = default;

    virtual void alphaTest(bool f);
    virtual void depthTest(bool f);
    virtual void blending(bool f);
    virtual void stencilTest(bool f);
    virtual void cullFace(bool f);

    virtual void transparencyMode(int16_t m);
    virtual void transparencyFactor(int16_t s, int16_t d);
};

typedef std::unique_ptr<GraphicStateFuncs> GraphicStateFuncsUPtr;

//-------------------------------------------------------------------------------------------------

/**
    \brief

    The purpose of this class is to manage graphic state such that redundant 
    state change calls will not be passed to the API.

    enable, disable, set* and transparency* function will update corresponding
    states in memory under this state manager. The only function that affect
    actual rendering is update, where states are pushed to graphics card.

 */
class GraphicStateManager {
public:
    GraphicStateManager(GraphicStateFuncs* graphicStateFuncs = nullptr);
    ~GraphicStateManager() = default;

    // update graphic state of the graphic API. Only update ones required
    void update();

    // enable/disable one state
    void enable(RenderPipelineStates rps);
    void disable(RenderPipelineStates rps);

    // replace all current states
    void setState(int16_t state);

    // change transparency function
    void transparencyMode(TransparencyModes to);
    void transparencyFactor(int16_t s, int16_t d);

private:
    void push();
    void cache();

private:
    // current state in the graphic engine
    GraphicState m_currentState;

    // state that is pushed to graphic drivers
    GraphicState m_pushedState;

    // function to manipulate states
    GraphicStateFuncsUPtr m_graphicStateFuncs;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_StateManager_h_
