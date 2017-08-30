#ifndef __YAVIN_TEXTURE_H__
#define __YAVIN_TEXTURE_H__

#include <iostream>
#include "Texture.h"
#include "Type.h"
#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {
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

  Texture() {}
  ~Texture() {
    if (!m_dont_delete) {
      glDeleteTextures(1, &m_id);
      gl_error_check("glDeleteTextures");
    }
  }

  const GLuint& id() const { return m_id; }

 protected:
  GLuint m_id;
  bool   m_dont_delete = false;
};
}  // namespace Yavin

#endif