#ifndef __YAVIN_TEXHELPER_H__
#define __YAVIN_TEXHELPER_H__

#include <stdint.h>
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

struct R {};
struct RG {};
struct RGB {};
struct RGBA {};

struct BGR {};
struct BGRA {};
struct Depth8 {};
struct Depth16 {};
struct Depth24 {};
struct Depth32 {};

//==============================================================================
namespace TexHelper {
//==============================================================================

template <typename T, typename format>
struct comb {};

/////// int8
template <>
struct comb<int8_t, R> {
  static constexpr GLint        internal_format = GL_R8;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<int8_t, RG> {
  static constexpr GLint        internal_format = GL_RG8;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<int8_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int8_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<int8_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int8_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 4;
};

/////// uint8
template <>
struct comb<uint8_t, R> {
  static constexpr GLint        internal_format = GL_R8;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<uint8_t, RG> {
  static constexpr GLint        internal_format = GL_RG8;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<uint8_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint8_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<uint8_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint8_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
  static constexpr unsigned int num_components  = 4;
};

/////// int16
template <>
struct comb<int16_t, R> {
  static constexpr GLint        internal_format = GL_R16;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<int16_t, RG> {
  static constexpr GLint        internal_format = GL_RG16;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<int16_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int16_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<int16_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int16_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_SHORT;
  static constexpr unsigned int num_components  = 4;
};

/////// uint16
template <>
struct comb<uint16_t, R> {
  static constexpr GLint        internal_format = GL_R16;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<uint16_t, RG> {
  static constexpr GLint        internal_format = GL_RG16;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<uint16_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint16_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<uint16_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint16_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
  static constexpr unsigned int num_components  = 4;
};

/////// int32
template <>
struct comb<int32_t, R> {
  static constexpr GLint        internal_format = GL_R32I;
  static constexpr GLenum       format          = GL_RED_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<int32_t, RG> {
  static constexpr GLint        internal_format = GL_RG32I;
  static constexpr GLenum       format          = GL_RG_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<int32_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB32I;
  static constexpr GLenum       format          = GL_RGB_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int32_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32I;
  static constexpr GLenum       format          = GL_RGBA_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<int32_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB32I;
  static constexpr GLenum       format          = GL_BGR_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<int32_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32I;
  static constexpr GLenum       format          = GL_BGRA_INTEGER;
  static constexpr GLenum       type            = GL_INT;
  static constexpr unsigned int num_components  = 4;
};

/////// uint32
template <>
struct comb<uint32_t, R> {
  static constexpr GLint        internal_format = GL_R32UI;
  static constexpr GLenum       format          = GL_RED_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<uint32_t, RG> {
  static constexpr GLint        internal_format = GL_RG32UI;
  static constexpr GLenum       format          = GL_RG_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<uint32_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB32UI;
  static constexpr GLenum       format          = GL_RGB_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint32_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32UI;
  static constexpr GLenum       format          = GL_RGBA_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<uint32_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB32UI;
  static constexpr GLenum       format          = GL_BGR_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<uint32_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32UI;
  static constexpr GLenum       format          = GL_BGRA_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
  static constexpr unsigned int num_components  = 4;
};

/////// FLOAT
template <>
struct comb<float, R> {
  static constexpr GLint        internal_format = GL_R32F;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<float, RG> {
  static constexpr GLint        internal_format = GL_RG32F;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 2;
};

template <>
struct comb<float, RGB> {
  static constexpr GLint        internal_format = GL_RGB32F;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<float, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32F;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 4;
};

template <>
struct comb<float, BGR> {
  static constexpr GLint        internal_format = GL_RGB32F;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 3;
};

template <>
struct comb<float, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32F;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 4;
};

/////// Depth
template <>
struct comb<int8_t, Depth8> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<int16_t, Depth16> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT16;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<float, Depth24> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT24;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 1;
};

template <>
struct comb<float, Depth32> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT32;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_FLOAT;
  static constexpr unsigned int num_components  = 1;
};
//==============================================================================
}  // namespace TexHelper
//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
