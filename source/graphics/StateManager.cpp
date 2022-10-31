//-------------------------------------------------------------------------------------------------
#include <graphics/StateManager.h>
//-------------------------------------------------------------------------------------------------
#include <cassert>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------
inline int16_t addState(int16_t& inputState, int16_t statesToAdd)
{
    inputState = inputState | statesToAdd;
    return inputState;
}

inline int16_t removeState(int16_t& inputState, int16_t statesToRemove)
{
    inputState = inputState & ~statesToRemove;
    return inputState;
}

bool hasState(int16_t state, int16_t stateToCheck)
{
    return (state & stateToCheck) == stateToCheck;
}
//-------------------------------------------------------------------------------------------------

GraphicStateManager::GraphicStateManager(GraphicStateFuncs* graphicStateFuncs)
    : m_graphicStateFuncs(graphicStateFuncs)
{
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::update()
{
    // push states to graphics card, and cache current state for reference later
    push();
    cache();
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::push()
{
    int16_t previous = m_pushedState.states;
    int16_t current = m_currentState.states;

    if (previous == current)
        return;

    // for each state, see if an update to graphic driver is
    // required.

    bool useAlphaTest = hasState(current, RPS_ALPHA_TEST);
    bool useDepthTest = hasState(current, RPS_DEPTH_TEST);
    bool useStencilTest = hasState(current, RPS_STENCIL_TEST);
    bool useBlending = hasState(current, RPS_BLENDING);
    bool useCullFace = hasState(current, RPS_CULL_FACE);

    bool prevUseAlphaTest = hasState(previous, RPS_ALPHA_TEST);
    bool prevUseDepthTest = hasState(previous, RPS_DEPTH_TEST);
    bool prevUseStencilTest = hasState(previous, RPS_STENCIL_TEST);
    bool prevUseBlending = hasState(previous, RPS_BLENDING);
    bool prevUseCullFace = hasState(previous, RPS_CULL_FACE);

    if (useAlphaTest != prevUseAlphaTest) {
        m_graphicStateFuncs->alphaTest(useAlphaTest);
    }

    if (useDepthTest != prevUseDepthTest) {
        m_graphicStateFuncs->depthTest(useDepthTest);
    }

    if (useStencilTest != prevUseStencilTest) {
        m_graphicStateFuncs->stencilTest(useStencilTest);
    }

    if (useBlending) {
        if (useBlending != prevUseBlending) {
            m_graphicStateFuncs->blending(useBlending);
        }

        if (m_pushedState.transparencyMode != m_currentState.transparencyMode) {
            m_graphicStateFuncs->transparencyMode(m_currentState.transparencyMode);
        }
    }

    if (useCullFace != prevUseCullFace) {
        m_graphicStateFuncs->cullFace(useCullFace);
    }
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::cache()
{
    m_pushedState = m_currentState;
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::enable(RenderPipelineStates rps)
{
    addState(m_currentState.states, rps);
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::disable(RenderPipelineStates rps)
{
    removeState(m_currentState.states, rps);
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::setState(int16_t state)
{
    m_currentState.states = state;
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::transparencyMode(TransparencyModes to)
{
    m_currentState.transparencyMode = to;
}

//-------------------------------------------------------------------------------------------------

void GraphicStateManager::transparencyFactor(int16_t s, int16_t d)
{
    m_currentState.srcFactor = s;
    m_currentState.dstFactor = d;
}

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Default functions are not implemented. Users are expect to provide these functions.
//-------------------------------------------------------------------------------------------------

void GraphicStateFuncs::alphaTest(bool f)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::depthTest(bool f)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::blending(bool f)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::stencilTest(bool f)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::cullFace(bool f)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::transparencyMode(int16_t s)
{
    assert(false && "Not implemented");
}

void GraphicStateFuncs::transparencyFactor(int16_t s, int16_t d)
{
    assert(false && "Not implemented");
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
