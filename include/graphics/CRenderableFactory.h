//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_CRenderableFactory_h
#define DKE_Graphics_CRenderableFactory_h
//-------------------------------------------------------------------------------------------------
#include "graphics/IRenderableFactory.h"
#include "graphics/Renderable.h"
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

/*!
 * \class   CRenderableFactory
 * \brief   Internal helper to create templated concrete renderable instance`.
 */
template <class T>
class CRenderableFactory : public IRenderableFactory {
public:
    static_assert(std::is_base_of<Renderable, T>::value, "Template class A must derive from Renderable");

    virtual Renderable* create() const override
    {
        return dynamic_cast<Renderable*>(new T);
    }
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_CRenderableFactory_h
