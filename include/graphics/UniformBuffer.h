//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_UniformBuffer_h
#define DKE_Graphics_UniformBuffer_h
//-------------------------------------------------------------------------------------------------
#include <graphics/Buffer.h>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

/*!
 * UniformBuffer is a subset of Buffer that handles uniform buffers / buffers storing
 * shader parameter blocks in graphics API. 
 *
 * It is named as UniformBuffer now to deal with uniform buffer operations, but
 * it should be able to handle uniform/atomic counter/transform feedback/shader storage
 * buffers as well.
 */
class UniformBuffer {
public:
    UniformBuffer(uint32_t target, size_t size, uint32_t bindingPoint);
    virtual ~UniformBuffer();

    virtual void bindBase();
    virtual void bindRange(uint32_t start, uint32_t size);

protected:
    UniformBuffer(const UniformBuffer& b) = delete;

protected:
    uint32_t m_bindingPoint;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_UniformBuffer_h
