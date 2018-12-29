#ifndef __YAVIN_GL_WRAPPER_H__
#define __YAVIN_GL_WRAPPER_H__

#include <glm/glm.hpp>
#include "camera.h"
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace yavin {
//==============================================================================

DLL_API void clear_color_buffer();
DLL_API void clear_depth_Buffer();
DLL_API void clear_color_depth_buffer();

DLL_API void enable_depth_test();
DLL_API void disable_depth_test();

DLL_API void enable_depth_write();
DLL_API void disable_depth_write();

DLL_API void enable_blending();
DLL_API void disable_blending();

DLL_API void blend_func_additive();
DLL_API void blend_func_multiplicative();
DLL_API void blend_func_subtractive();
DLL_API void blend_func_alpha();

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
