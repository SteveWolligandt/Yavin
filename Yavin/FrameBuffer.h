#ifndef __YAVIN_FRAMEBUFFER_H__
#define __YAVIN_FRAMEBUFFER_H__

#include "error_check.h"

#include "Texture2D.h"

namespace Yavin {
class FrameBuffer {
 public:
  FrameBuffer();
  FrameBuffer(FrameBuffer&& other);
  ~FrameBuffer();

  template <typename T, typename Components>
  void attachTex2D(const Texture2D<T, Components>& tex, unsigned int i = 0);

  void        bind();
  static void unbind();

  void clear();

  GLuint id() const;

 private:
  GLuint m_id;
  bool   m_dont_delete = false;
};
}  // Yavin

#endif