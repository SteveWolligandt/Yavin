#ifndef __YAVIN_GL_WRAPPER_H__
#define __YAVIN_GL_WRAPPER_H__

#include <glm/glm.hpp>
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {
DLL_API void        clear_color_buffer();
DLL_API void        clear_depth_Buffer();
DLL_API void        clear_color_depth_buffer();
DLL_API void        set_clear_color(float r, float g, float b, float a);
DLL_API void        flush();
DLL_API void        enable_depth_test();
DLL_API void        disable_depth_test();
DLL_API void        enable_depth_write();
DLL_API void        disable_depth_write();
DLL_API void        enable_blending();
DLL_API void        disable_blending();
DLL_API void        blend_func(GLenum sfactor, GLenum dfactor);
DLL_API void        blend_func_additive();
DLL_API void        blend_func_multiplicative();
DLL_API void        blend_func_subtractive();
DLL_API void        blend_func_alpha();
DLL_API void        set_viewport(unsigned int left, unsigned int bottom,
                                 unsigned int width, unsigned int height);
DLL_API inline void set_viewport(const glm::vec4& vp) {
  set_viewport((unsigned int)vp[0], (unsigned int)vp[1], (unsigned int)vp[2],
               (unsigned int)vp[3]);
}
}  // namespace Yavin

#endif
