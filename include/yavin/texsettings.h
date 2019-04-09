#ifndef __YAVIN_TEX_SETTINGS_H__
#define  __YAVIN_TEX_SETTINGS_H__

#include <cstdint>
#include "texcomponents.h"

//==============================================================================
namespace yavin::tex {
//==============================================================================

template <typename T, typename format>
struct settings;

//------------------------------------------------------------------------------
// int8
template <>
struct settings<int8_t, R> {
  static constexpr GLint  internal_format = GL_R8;
  static constexpr GLenum format          = GL_RED;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int8_t, RG> {
  static constexpr GLint  internal_format = GL_RG8;
  static constexpr GLenum format          = GL_RG;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int8_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB8;
  static constexpr GLenum format          = GL_RGB;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int8_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA8;
  static constexpr GLenum format          = GL_RGBA;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int8_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB8;
  static constexpr GLenum format          = GL_BGR;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int8_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA8;
  static constexpr GLenum format          = GL_BGRA;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//------------------------------------------------------------------------------
// uint8
template <>
struct settings<uint8_t, R> {
  static constexpr GLint  internal_format = GL_R8;
  static constexpr GLenum format          = GL_RED;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint8_t, RG> {
  static constexpr GLint  internal_format = GL_RG8;
  static constexpr GLenum format          = GL_RG;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint8_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB8;
  static constexpr GLenum format          = GL_RGB;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint8_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA8;
  static constexpr GLenum format          = GL_RGBA;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint8_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB8;
  static constexpr GLenum format          = GL_BGR;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint8_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA8;
  static constexpr GLenum format          = GL_BGRA;
  static constexpr GLenum type            = GL_UNSIGNED_BYTE;
};

//------------------------------------------------------------------------------
// int16
template <>
struct settings<int16_t, R> {
  static constexpr GLint  internal_format = GL_R16;
  static constexpr GLenum format          = GL_RED;
  static constexpr GLenum type            = GL_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int16_t, RG> {
  static constexpr GLint  internal_format = GL_RG16;
  static constexpr GLenum format          = GL_RG;
  static constexpr GLenum type            = GL_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int16_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB16;
  static constexpr GLenum format          = GL_RGB;
  static constexpr GLenum type            = GL_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int16_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA16;
  static constexpr GLenum format          = GL_RGBA;
  static constexpr GLenum type            = GL_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int16_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB16;
  static constexpr GLenum format          = GL_BGR;
  static constexpr GLenum type            = GL_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int16_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA16;
  static constexpr GLenum format          = GL_BGRA;
  static constexpr GLenum type            = GL_SHORT;
};

//------------------------------------------------------------------------------
// uint16
template <>
struct settings<uint16_t, R> {
  static constexpr GLint  internal_format = GL_R16;
  static constexpr GLenum format          = GL_RED;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint16_t, RG> {
  static constexpr GLint  internal_format = GL_RG16;
  static constexpr GLenum format          = GL_RG;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint16_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB16;
  static constexpr GLenum format          = GL_RGB;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint16_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA16;
  static constexpr GLenum format          = GL_RGBA;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint16_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB16;
  static constexpr GLenum format          = GL_BGR;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint16_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA16;
  static constexpr GLenum format          = GL_BGRA;
  static constexpr GLenum type            = GL_UNSIGNED_SHORT;
};

//------------------------------------------------------------------------------
// int32
template <>
struct settings<int32_t, R> {
  static constexpr GLint  internal_format = GL_R32I;
  static constexpr GLenum format          = GL_RED_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int32_t, RG> {
  static constexpr GLint  internal_format = GL_RG32I;
  static constexpr GLenum format          = GL_RG_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int32_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB32I;
  static constexpr GLenum format          = GL_RGB_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int32_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA32I;
  static constexpr GLenum format          = GL_RGBA_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int32_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB32I;
  static constexpr GLenum format          = GL_BGR_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<int32_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA32I;
  static constexpr GLenum format          = GL_BGRA_INTEGER;
  static constexpr GLenum type            = GL_INT;
};

//------------------------------------------------------------------------------
// uint32
template <>
struct settings<uint32_t, R> {
  static constexpr GLint  internal_format = GL_R32UI;
  static constexpr GLenum format          = GL_RED_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint32_t, RG> {
  static constexpr GLint  internal_format = GL_RG32UI;
  static constexpr GLenum format          = GL_RG_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint32_t, RGB> {
  static constexpr GLint  internal_format = GL_RGB32UI;
  static constexpr GLenum format          = GL_RGB_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint32_t, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA32UI;
  static constexpr GLenum format          = GL_RGBA_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint32_t, BGR> {
  static constexpr GLint  internal_format = GL_RGB32UI;
  static constexpr GLenum format          = GL_BGR_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<uint32_t, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA32UI;
  static constexpr GLenum format          = GL_BGRA_INTEGER;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//------------------------------------------------------------------------------
// float
template <>
struct settings<float, R> {
  static constexpr GLint  internal_format = GL_R32F;
  static constexpr GLenum format          = GL_RED;
  static constexpr GLenum type            = GL_FLOAT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, RG> {
  static constexpr GLint  internal_format = GL_RG32F;
  static constexpr GLenum format          = GL_RG;
  static constexpr GLenum type            = GL_FLOAT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, RGB> {
  static constexpr GLint  internal_format = GL_RGB32F;
  static constexpr GLenum format          = GL_RGB;
  static constexpr GLenum type            = GL_FLOAT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, RGBA> {
  static constexpr GLint  internal_format = GL_RGBA32F;
  static constexpr GLenum format          = GL_RGBA;
  static constexpr GLenum type            = GL_FLOAT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, BGR> {
  static constexpr GLint  internal_format = GL_RGB32F;
  static constexpr GLenum format          = GL_BGR;
  static constexpr GLenum type            = GL_FLOAT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, BGRA> {
  static constexpr GLint  internal_format = GL_RGBA32F;
  static constexpr GLenum format          = GL_BGRA;
  static constexpr GLenum type            = GL_FLOAT;
};

//------------------------------------------------------------------------------
// Depth
template <>
struct settings<uint32_t, Depth> {
  static constexpr GLint  internal_format = GL_DEPTH_COMPONENT32;
  static constexpr GLenum format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum type            = GL_UNSIGNED_INT;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <>
struct settings<float, Depth> {
  static constexpr GLint  internal_format = GL_DEPTH_COMPONENT32;
  static constexpr GLenum format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum type            = GL_FLOAT;
};

//==============================================================================
}  // namespace yavin::tex
//==============================================================================

#endif
