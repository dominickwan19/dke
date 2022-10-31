//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderStates_h_
#define DKE_Graphics_RenderStates_h_
//-------------------------------------------------------------------------------------------------

#include <cstdint>
#include <stdio.h>

namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

/**
    Classes for managing states for rendering.
 */

struct TransparencyStates {
    enum {
        TRANSPARENCY_NONE = 0,
        TRANSPARENCY_NORMAL, // srcColor * a + dstColor * (1 - a)
        TRANSPARENCY_ADD,
        TRANSPARENCY_SUBTRACT
    };

    static void updateState(int8_t current, int8_t previous)
    {
        if (current == previous)
            return;

        printf("============================= Changing transparency state!\n");

        //if (previous != 0)
        //glEnable(GL_BLEND);

        switch (current) {
        case TRANSPARENCY_NONE:
            // glDisable(GL_BLEND);
            break;
        case TRANSPARENCY_NORMAL:
            // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case TRANSPARENCY_ADD:
            // glBlendFunc(GL_ONE, GL_ONE);
            break;
        case TRANSPARENCY_SUBTRACT:
            // glBlendFunc(??)
            break;
        }
    }
};

struct PipelineStates {
    static void updateState(int8_t current, int8_t previous)
    {
        if (current == previous)
            return;

        bool useAlphaTest = (current & 0x4) == 0x4;
        bool useDepthTest = (current & 0x2) == 0x2;
        bool useStencilTest = (current & 0x1) == 0x1;

        bool prevUseAlphaTest = (previous & 0x4) == 0x4;
        bool prevUseDepthTest = (previous & 0x2) == 0x2;
        bool prevUseStencilTest = (previous & 0x1) == 0x1;

        if (useAlphaTest != prevUseAlphaTest) {
            alphaTest(useAlphaTest);
        }
        if (useDepthTest != prevUseDepthTest) {
            depthTest(useDepthTest);
        }
        if (useStencilTest != prevUseStencilTest) {
            stencilTest(useStencilTest);
        }
    }

    static void alphaTest(bool a) {}
    static void depthTest(bool d) {}
    static void stencilTest(bool s) {}
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderStates_h_
