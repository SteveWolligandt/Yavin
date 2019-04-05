#ifndef __YAVIN_FRAMEBUFFER_H__
#define __YAVIN_FRAMEBUFFER_H__

#include "error_check.h"

#include "texture_2d.h"
#include "dll_export.h"

//==============================================================================
namespace yavin {
//==============================================================================

class FrameBuffer {
 public:
  DLL_API FrameBuffer();
  DLL_API FrameBuffer(FrameBuffer&& other);
  DLL_API ~FrameBuffer();

  template <typename T, typename Components>
  DLL_API void attachTex2D(const Texture2D<T, Components>& tex,
                           unsigned int                    i = 0);
  template <typename T>
  DLL_API void attachDepth(const Texture2D<T, Depth>& depth_tex);

  DLL_API void        bind();
  DLL_API static void unbind();

  DLL_API void clear();

  DLL_API GLuint id() const;

 private:
  GLuint m_id;
  bool   m_dont_delete = false;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif