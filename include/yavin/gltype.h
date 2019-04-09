#ifndef __YAVIN_GL_TYPE_H__
#define __YAVIN_GL_TYPE_H__

#include <cstdint>
#include "glincludes.h"

//==============================================================================
namespace yavin {
//==============================================================================

template <typename T>
struct gl_type;

//------------------------------------------------------------------------------

template <>
struct gl_type<float> {
  static constexpr GLenum type = GL_FLOAT;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<double> {
  static constexpr GLenum type = GL_DOUBLE;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<int8_t> {
  static constexpr GLenum type = GL_BYTE;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<uint8_t> {
  static constexpr GLenum type = GL_UNSIGNED_BYTE;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<int16_t> {
  static constexpr GLenum type = GL_INT;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<uint16_t> {
  static constexpr GLenum type = GL_UNSIGNED_SHORT;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<int32_t> {
  static constexpr GLenum type = GL_SHORT;
};

//------------------------------------------------------------------------------

template <>
struct gl_type<uint32_t> {
  static constexpr GLenum type = GL_UNSIGNED_INT;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
