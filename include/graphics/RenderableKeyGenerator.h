//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_RenderableKeyGenerator_h
#define DKE_Graphics_RenderableKeyGenerator_h
//-------------------------------------------------------------------------------------------------
#include "Renderable.h"
#include <bitset>
#include <cstdint>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Renderable;

class RenderableKeyGenerator {
public:
    RenderableKeyGenerator() = default;
    virtual ~RenderableKeyGenerator() = default;
    RenderableKeyGenerator(const RenderableKeyGenerator&) = delete;

    virtual uint64_t generateKey(const Renderable* renderable) = 0;
};

///**
//\brief Generates a 64 bit mask with pow number of least significant bits set as 1.
//*/
//inline unsigned long long operator"" _bitmask(unsigned long long pow)
//{
//    return (1 << pow) - 1;
//}

inline unsigned long long bitmask(unsigned long long pow)
{
    return (1 << pow) - 1;
}

class DefaultRenderableKeyGenerator : public RenderableKeyGenerator {
public:
    virtual uint64_t generateKey(const Renderable* renderable) override
    {
        uint64_t key = 0;

        uint64_t maskedValue = 0;
        maskedValue = uint64_t(renderable->category() & bitmask(3)) << 61;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->component() & bitmask(7)) << 58;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->entityType() & bitmask(7)) << 55;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->viewportId() & bitmask(8)) << 47;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->translucency() & bitmask(2)) << 45;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->states() & bitmask(3)) << 42;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->order() & bitmask(24)) << 18;
        key |= maskedValue;
        maskedValue = uint64_t(renderable->material() & bitmask(18));
        key |= maskedValue;

        return key;
    }
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_RenderableKeyGenerator_h
