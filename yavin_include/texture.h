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
//==============================================================================
namespace tex {
enum WrapMode {
  // CLAMP = GL_CLAMP,
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
}  // namespace tex

template <GLenum tex_t>
class BaseTexture : public id_holder<GLuint> {
 public:
  BaseTexture() { gl::create_textures(tex_t, 1, &this->m_id); }
  BaseTexture(BaseTexture&& other) : id_holder{std::move(other)} {}
  ~BaseTexture() {
    if (m_id) { gl::delete_textures(1, &m_id); }
  }
};

template <GLenum tex_t>
inline void swap(BaseTexture<tex_t>& t0, BaseTexture<tex_t>& t1) {
  t0.swap(t1);
}

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
