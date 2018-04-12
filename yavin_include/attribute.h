#ifndef __YAVIN_ATTRIBUTE_H__
#define __YAVIN_ATTRIBUTE_H__

#include "gl_includes.h"
#include "gl_type.h"

//==============================================================================
namespace Yavin {
//==============================================================================

/**
 * @brief      Base Attribute
 *
 * @tparam     Ts    Data in attribute
 */
template <unsigned int _n, typename T>
struct attrib {
  static constexpr GLenum       type     = gl_type<T>::type;
  static constexpr unsigned int num_dims = _n;
  T                             data[_n];
};

//------------------------------------------------------------------------------

template <typename T>
using scalar = attrib<1, T>;
template <typename T>
using vec2 = attrib<2, T>;
template <typename T>
using vec3 = attrib<3, T>;
template <typename T>
using vec4 = attrib<4, T>;

//------------------------------------------------------------------------------

using scalar_f = attrib<1, float>;
using vec2_f   = attrib<2, float>;
using vec3_f   = attrib<3, float>;
using vec4_f   = attrib<4, float>;

//------------------------------------------------------------------------------

using scalar_d = attrib<1, double>;
using vec2_d   = attrib<2, double>;
using vec3_d   = attrib<3, double>;
using vec4_d   = attrib<4, double>;

//------------------------------------------------------------------------------

using scalar_i32 = attrib<1, int32_t>;
using vec2_i32   = attrib<2, int32_t>;
using vec3_i32   = attrib<3, int32_t>;
using vec4_i32   = attrib<4, int32_t>;

//------------------------------------------------------------------------------

using scalar_ui32 = attrib<1, uint32_t>;
using vec2_ui32   = attrib<2, uint32_t>;
using vec3_ui32   = attrib<3, uint32_t>;
using vec4_ui32   = attrib<4, uint32_t>;

//------------------------------------------------------------------------------

using scalar_i16 = attrib<1, int16_t>;
using vec2_i16   = attrib<2, int16_t>;
using vec3_i16   = attrib<3, int16_t>;
using vec4_i16   = attrib<4, int16_t>;

//------------------------------------------------------------------------------

using scalar_ui16 = attrib<1, uint16_t>;
using vec2_ui16   = attrib<2, uint16_t>;
using vec3_ui16   = attrib<3, uint16_t>;
using vec4_ui16   = attrib<4, uint16_t>;

//------------------------------------------------------------------------------

using scalar_i8 = attrib<1, int8_t>;
using vec2_i8   = attrib<2, int8_t>;
using vec3_i8   = attrib<3, int8_t>;
using vec4_i8   = attrib<4, int8_t>;

//------------------------------------------------------------------------------

using scalar_ui8 = attrib<1, uint8_t>;
using vec2_ui8   = attrib<2, uint8_t>;
using vec3_ui8   = attrib<3, uint8_t>;
using vec4_ui8   = attrib<4, uint8_t>;

//==============================================================================
}  // namespace Yavin
//==============================================================================
#endif
