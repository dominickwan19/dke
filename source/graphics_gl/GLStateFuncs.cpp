#include <ex_rendering_platform.h>
#include <rendering/gles/GLStateFuncs.h>

void GLStateFuncs::alphaTest(bool f) {
    //if (f)
    //    glEnable(GL_ALPHA_TEST);
    //else
    //    glDisable(GL_ALPHA_TEST);
}

void GLStateFuncs::depthTest(bool f) {
    if (f)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

}

void GLStateFuncs::blending(bool f) {
    if (f)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

}

void GLStateFuncs::transparencyMode(int16_t s) {

}

void GLStateFuncs::transparencyFactor(int16_t s, int16_t d) {
    glBlendFunc(s, d);
}

void GLStateFuncs::stencilTest(bool f) {
    if (f)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}

void GLStateFuncs::cullFace(bool f) {
    if (f)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}
