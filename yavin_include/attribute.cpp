#include "attribute.h"

//==============================================================================
namespace yavin {
//==============================================================================

template struct attrib<1, float>;
template struct attrib<2, float>;
template struct attrib<3, float>;
template struct attrib<4, float>;

template struct attrib<1, double>;
template struct attrib<2, double>;
template struct attrib<3, double>;
template struct attrib<4, double>;

template struct attrib<1, int32_t>;
template struct attrib<2, int32_t>;
template struct attrib<3, int32_t>;
template struct attrib<4, int32_t>;

template struct attrib<1, uint32_t>;
template struct attrib<2, uint32_t>;
template struct attrib<3, uint32_t>;
template struct attrib<4, uint32_t>;

template struct attrib<1, int16_t>;
template struct attrib<2, int16_t>;
template struct attrib<3, int16_t>;
template struct attrib<4, int16_t>;

template struct attrib<1, uint16_t>;
template struct attrib<2, uint16_t>;
template struct attrib<3, uint16_t>;
template struct attrib<4, uint16_t>;

template struct attrib<1, int8_t>;
template struct attrib<2, int8_t>;
template struct attrib<3, int8_t>;
template struct attrib<4, int8_t>;

template struct attrib<1, uint8_t>;
template struct attrib<2, uint8_t>;
template struct attrib<3, uint8_t>;
template struct attrib<4, uint8_t>;

//==============================================================================
}  // namespace yavin
//==============================================================================
