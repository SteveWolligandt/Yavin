#ifndef __YAVIN_TEXTURE_H__
#define __YAVIN_TEXTURE_H__

#include <iostream>
#include <utility>
#include "texture.h"
#include "type.h"
#include "error_check.h"
#include "gl_functions.h"

//==============================================================================
namespace yavin {
//==============================================================================

class Texture {
 public:
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

  Texture() : m_id{0} {}
  Texture(Texture&& other) : m_id{std::exchange(other.m_id, 0)} {}
  ~Texture() {
    if (m_id) { gl::delete_textures(1, &m_id); }
  }

  auto id() const { return m_id; }

 protected:
  GLuint m_id;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
