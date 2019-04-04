#ifndef __YAVIN_TEXTURE_H__
#define __YAVIN_TEXTURE_H__

#include <iostream>
#include <utility>
#include "error_check.h"
#include "gl_functions.h"
#include "id_holder.h"
#include "texture.h"
#include "type.h"

//==============================================================================
namespace yavin {

struct R {
  static constexpr size_t num_components = 1;
};
struct RG {
  static constexpr size_t num_components = 2;
};
struct RGB {
  static constexpr size_t num_components = 3;
};
struct RGBA {
  static constexpr size_t num_components = 4;
};
struct BGR {
  static constexpr size_t num_components = 3;
};
struct BGRA {
  static constexpr size_t num_components = 4;
};
struct Depth {
  static constexpr size_t num_components = 1;
};
//==============================================================================
namespace tex {
//==============================================================================
enum WrapMode {
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
  REPEAT          = GL_REPEAT,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

enum InterpolationMode {
  NEAREST                = GL_NEAREST,
  LINEAR                 = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};

enum CompareFunc {
  NEVER    = GL_NEVER,
  LESS     = GL_LESS,
  LEQUAL   = GL_LEQUAL,
  GREATER  = GL_GREATER,
  NOTEQUAL = GL_NOTEQUAL,
  GEQUAL   = GL_GEQUAL,
  ALWAYS   = GL_ALWAYS
};

enum CompareMode {
  COMPARE_R_TO_TEXTURE = GL_COMPARE_R_TO_TEXTURE,
  NONE                 = GL_NONE
};

template <typename T, typename format>
struct comb {};

/////// int8
template <>
struct comb<int8_t, R> {
  static constexpr GLint        internal_format = GL_R8;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<int8_t, RG> {
  static constexpr GLint        internal_format = GL_RG8;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<int8_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<int8_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<int8_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<int8_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

/////// uint8
template <>
struct comb<uint8_t, R> {
  static constexpr GLint        internal_format = GL_R8;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<uint8_t, RG> {
  static constexpr GLint        internal_format = GL_RG8;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<uint8_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<uint8_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<uint8_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB8;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

template <>
struct comb<uint8_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA8;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_BYTE;
};

/////// int16
template <>
struct comb<int16_t, R> {
  static constexpr GLint        internal_format = GL_R16;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_SHORT;
};

template <>
struct comb<int16_t, RG> {
  static constexpr GLint        internal_format = GL_RG16;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_SHORT;
};

template <>
struct comb<int16_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_SHORT;
};

template <>
struct comb<int16_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_SHORT;
};

template <>
struct comb<int16_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_SHORT;
};

template <>
struct comb<int16_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_SHORT;
};

/////// uint16
template <>
struct comb<uint16_t, R> {
  static constexpr GLint        internal_format = GL_R16;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

template <>
struct comb<uint16_t, RG> {
  static constexpr GLint        internal_format = GL_RG16;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

template <>
struct comb<uint16_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

template <>
struct comb<uint16_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

template <>
struct comb<uint16_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB16;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

template <>
struct comb<uint16_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA16;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_UNSIGNED_SHORT;
};

/////// int32
template <>
struct comb<int32_t, R> {
  static constexpr GLint        internal_format = GL_R32I;
  static constexpr GLenum       format          = GL_RED_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

template <>
struct comb<int32_t, RG> {
  static constexpr GLint        internal_format = GL_RG32I;
  static constexpr GLenum       format          = GL_RG_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

template <>
struct comb<int32_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB32I;
  static constexpr GLenum       format          = GL_RGB_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

template <>
struct comb<int32_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32I;
  static constexpr GLenum       format          = GL_RGBA_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

template <>
struct comb<int32_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB32I;
  static constexpr GLenum       format          = GL_BGR_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

template <>
struct comb<int32_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32I;
  static constexpr GLenum       format          = GL_BGRA_INTEGER;
  static constexpr GLenum       type            = GL_INT;
};

/////// uint32
template <>
struct comb<uint32_t, R> {
  static constexpr GLint        internal_format = GL_R32UI;
  static constexpr GLenum       format          = GL_RED_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<uint32_t, RG> {
  static constexpr GLint        internal_format = GL_RG32UI;
  static constexpr GLenum       format          = GL_RG_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<uint32_t, RGB> {
  static constexpr GLint        internal_format = GL_RGB32UI;
  static constexpr GLenum       format          = GL_RGB_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<uint32_t, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32UI;
  static constexpr GLenum       format          = GL_RGBA_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<uint32_t, BGR> {
  static constexpr GLint        internal_format = GL_RGB32UI;
  static constexpr GLenum       format          = GL_BGR_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<uint32_t, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32UI;
  static constexpr GLenum       format          = GL_BGRA_INTEGER;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

/////// FLOAT
template <>
struct comb<float, R> {
  static constexpr GLint        internal_format = GL_R32F;
  static constexpr GLenum       format          = GL_RED;
  static constexpr GLenum       type            = GL_FLOAT;
};

template <>
struct comb<float, RG> {
  static constexpr GLint        internal_format = GL_RG32F;
  static constexpr GLenum       format          = GL_RG;
  static constexpr GLenum       type            = GL_FLOAT;
};

template <>
struct comb<float, RGB> {
  static constexpr GLint        internal_format = GL_RGB32F;
  static constexpr GLenum       format          = GL_RGB;
  static constexpr GLenum       type            = GL_FLOAT;
};

template <>
struct comb<float, RGBA> {
  static constexpr GLint        internal_format = GL_RGBA32F;
  static constexpr GLenum       format          = GL_RGBA;
  static constexpr GLenum       type            = GL_FLOAT;
};

template <>
struct comb<float, BGR> {
  static constexpr GLint        internal_format = GL_RGB32F;
  static constexpr GLenum       format          = GL_BGR;
  static constexpr GLenum       type            = GL_FLOAT;
};

template <>
struct comb<float, BGRA> {
  static constexpr GLint        internal_format = GL_RGBA32F;
  static constexpr GLenum       format          = GL_BGRA;
  static constexpr GLenum       type            = GL_FLOAT;
};

/////// Depth
template <>
struct comb<uint32_t, Depth> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT32;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_UNSIGNED_INT;
};

template <>
struct comb<float, Depth> {
  static constexpr GLint        internal_format = GL_DEPTH_COMPONENT32;
  static constexpr GLenum       format          = GL_DEPTH_COMPONENT;
  static constexpr GLenum       type            = GL_FLOAT;
};
//==============================================================================
}  // namespace tex
//==============================================================================

template <GLenum tex_t, typename T, typename C>
class BaseTexture : public id_holder<GLuint> {
 public:
  //============================================================================
  using type = T;
  using components = C;
  static constexpr bool is_float = std::is_same<float, type>::value;
  static constexpr auto default_interpolation = tex::LINEAR;
  static constexpr auto default_wrap_mode     = tex::REPEAT;
  static constexpr auto num_components        = components::num_components;
  static constexpr auto gl_internal_format = tex::comb<T, C>::internal_format;
  static constexpr auto gl_format          = tex::comb<T, C>::format;
  static constexpr auto gl_type            = tex::comb<T, C>::type;

  //============================================================================
  BaseTexture() { gl::create_textures(tex_t, 1, &this->m_id); }

  //----------------------------------------------------------------------------
  BaseTexture(BaseTexture&& other) : id_holder{std::move(other)} {}

  //----------------------------------------------------------------------------
  ~BaseTexture() {
    if (m_id) { gl::delete_textures(1, &m_id); }
  }
};

//------------------------------------------------------------------------------
template <GLenum tex_t, typename T, typename C>
  inline void swap(BaseTexture<tex_t, T, C>& t0, BaseTexture<tex_t, T, C>& t1) {
  t0.swap(t1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
