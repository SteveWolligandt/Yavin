#ifndef __YAVIN_FRAMEBUFFER_H__
#define __YAVIN_FRAMEBUFFER_H__

#include "error_check.h"

#include "dll_export.h"
#include "texture.h"

//==============================================================================
namespace yavin {
//==============================================================================

class framebuffer : public id_holder<GLuint> {
 public:
  DLL_API framebuffer();
  DLL_API ~framebuffer();
  template <typename... Textures>
  DLL_API framebuffer(const Textures&... textures) : framebuffer{} {
    unsigned int i = 0;
    using discard = int[];
    // attach textures one after another, incrementing i if texture is a color
    // texture
    (void)discard{((void)(std::is_same_v<typename Textures::components, Depth>
                              ? attach(textures, i)
                              : attach(textures, i++)),
                   0)...};
  }

  template <typename T, typename Components>
  DLL_API void attach(const tex2D<T, Components>& tex, unsigned int i = 0);
  template <typename T>
  DLL_API void attach(const tex2D<T, Depth>& depth_tex);
 private:
  // this is necessary for constructor taking variadic parameters
  template <typename T>
  DLL_API void attach(const tex2D<T, Depth>& depth_tex, unsigned int) {
    attach(depth_tex);
  }

 public:
  DLL_API void        bind();
  DLL_API static void unbind();

  DLL_API void clear();

 private:
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
