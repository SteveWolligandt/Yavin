#ifndef __YAVIN_TEXTURE_H__
#define __YAVIN_TEXTURE_H__

#include <iostream>
#include <utility>
#include "Texture.h"
#include "Type.h"
#include "error_check.h"
#include "gl_functions.h"

//==============================================================================
namespace Yavin {
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
}  // namespace Yavin
//==============================================================================

#endif
