//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_IRenderParameters_h
#define DKE_Graphics_IRenderParameters_h
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

/*!
 * \class   IRenderParameters
 * \brief   Interface for handling conversion from a piece of data to a structure and eventually to
 *          GPU friendly format.
 */
class IRenderParameters {
public:
    virtual ~IRenderParameters() = default;

    // TODO: consider if we want such a function. If yes we will want subclass to
    //       fill in data of structure represented by this class here.
    //virtual void populate();

    // Convert data represented by this class to buffer object friendly float array.
    virtual float* toFloatArray() = 0;
    
    // 
    // TODO: template function so that it calculates data size respective to all types?
    virtual size_t dataSize() = 0;
};

/*!
 * \class   CRenderParameters
 */
template <class T>
class CRenderParameters : public IRenderParameters {
public:
    CRenderParameters() {}

    virtual float* toFloatArray() override;
    virtual size_t dataSize() override;

public:
    union {
        T m_data;
        float m_dataArray[sizeof(T) / 4]; // TODO: align to 32 bit by calculating proper size
    };
};

template <class T>
float* CRenderParameters<T>::toFloatArray()
{
    return m_dataArray;
}

template <class T>
size_t CRenderParameters<T>::dataSize() {
    return sizeof(T) / 4;
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_IRenderParameters_h
