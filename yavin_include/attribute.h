#ifndef __YAVIN_ATTRIBUTE_H__
#define __YAVIN_ATTRIBUTE_H__

#include <ostream>
#include "gl_includes.h"
#include "gl_type.h"

//==============================================================================
namespace yavin {
//==============================================================================

//!  Base Attribute
template <unsigned int n, typename T>
struct attrib {
  static constexpr GLenum       type     = gl_type<T>::type;
  static constexpr unsigned int num_dims = n;
  T                             data[n];

  auto&       operator[](size_t i) { return data[i]; }
  const auto& operator[](size_t i) const { return data[i]; }

  const auto begin() const { return data; }
  const auto end() const { return data + n; }

  auto begin() { return data; }
  auto end() { return data + n; }
};

template <unsigned int n, typename T>
auto& operator<<(std::ostream& out, const attrib<n, T>& a) {
  out << "[ ";
  for (auto v : a) out << v << ' ';
  out << ']';
  return out;
}

//------------------------------------------------------------------------------

template <typename T>
using Scalar = attrib<1, T>;
template <typename T>
using Vec2 = attrib<2, T>;
template <typename T>
using Vec3 = attrib<3, T>;
template <typename T>
using Vec4 = attrib<4, T>;

//------------------------------------------------------------------------------

using scalar = attrib<1, float>;
using vec2   = attrib<2, float>;
using vec3   = attrib<3, float>;
using vec4   = attrib<4, float>;

//------------------------------------------------------------------------------

using dscalar = attrib<1, double>;
using dvec2   = attrib<2, double>;
using dvec3   = attrib<3, double>;
using dvec4   = attrib<4, double>;

//------------------------------------------------------------------------------

using i32scalar = attrib<1, int32_t>;
using i32vec2   = attrib<2, int32_t>;
using i32vec3   = attrib<3, int32_t>;
using i32vec4   = attrib<4, int32_t>;

//------------------------------------------------------------------------------

using ui32scalar = attrib<1, uint32_t>;
using ui32vec2   = attrib<2, uint32_t>;
using ui32vec3   = attrib<3, uint32_t>;
using ui32vec4   = attrib<4, uint32_t>;

//------------------------------------------------------------------------------

using i16scalar = attrib<1, int16_t>;
using i16vec2   = attrib<2, int16_t>;
using i16vec3   = attrib<3, int16_t>;
using i16vec4   = attrib<4, int16_t>;

//------------------------------------------------------------------------------

using ui16scalar = attrib<1, uint16_t>;
using ui16vec2   = attrib<2, uint16_t>;
using ui16vec3   = attrib<3, uint16_t>;
using ui16vec4   = attrib<4, uint16_t>;

//------------------------------------------------------------------------------

using i8scalar = attrib<1, int8_t>;
using i8vec2   = attrib<2, int8_t>;
using i8vec3   = attrib<3, int8_t>;
using i8vec4   = attrib<4, int8_t>;

//------------------------------------------------------------------------------

using ui8scalar = attrib<1, uint8_t>;
using ui8vec2   = attrib<2, uint8_t>;
using ui8vec3   = attrib<3, uint8_t>;
using ui8vec4   = attrib<4, uint8_t>;

//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
