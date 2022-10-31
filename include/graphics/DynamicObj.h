//-------------------------------------------------------------------------------------------------
#ifndef DynamicObj_h
#define DynamicObj_h
//-------------------------------------------------------------------------------------------------
#include <cstdint>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------
/*!
 * \class   DynamicObj
 * \brief   Record change time of an object for dynamic update.
 */
class DynamicObj {
public:
    DynamicObj() = default;

    void update();
    int64_t timestamp() const;
    bool isChangedSince(int64_t timestamp) const;

private:
    int64_t m_timestamp;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DynamicObj_h
