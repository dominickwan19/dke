//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_IRenderableFactory_h
#define DKE_Graphics_IRenderableFactory_h
//-------------------------------------------------------------------------------------------------
#include <memory>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

// Forward Declarations.
class Renderable;

//-------------------------------------------------------------------------------------------------

/*!
 * \class   IRenderableFactory
 * \brief   Internal common interface for all type of CRenderableFactory to work on.
 */
class IRenderableFactory {
public:
    virtual Renderable* create() const = 0;
};

typedef std::shared_ptr<IRenderableFactory> IRenderableFactoryPtr;

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_IRenderableFactory_h
