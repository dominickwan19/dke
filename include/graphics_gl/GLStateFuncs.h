#ifndef GLStateFuncs_h_
#define GLStateFuncs_h_

#include <graphics/StateManager.h>

class GLStateFuncs : public exgraphics::GraphicStateFuncs {
public:
    virtual void alphaTest(bool f) override;
    virtual void depthTest(bool f) override;
    virtual void blending(bool f) override;
    virtual void transparencyMode(int16_t m) override;
    virtual void transparencyFactor(int16_t s, int16_t d) override;
    virtual void stencilTest(bool f) override;
    virtual void cullFace(bool f) override;
};

#endif
