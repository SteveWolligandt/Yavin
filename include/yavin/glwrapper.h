#ifndef YAVIN_GL_WRAPPER_H
#define YAVIN_GL_WRAPPER_H

#include "camera.h"
#include "dllexport.h"
#include "errorcheck.h"
#include "glincludes.h"

//==============================================================================
namespace yavin {
//==============================================================================
DLL_API void clear_color_buffer();
DLL_API void clear_depth_buffer();
DLL_API void clear_color_depth_buffer();
//==============================================================================
DLL_API void enable_depth_test();
DLL_API void disable_depth_test();
//==============================================================================
DLL_API void enable_depth_write();
DLL_API void disable_depth_write();
//==============================================================================
DLL_API void enable_multisampling();
DLL_API void disable_multisampling();
DLL_API bool multisampling_enabled();
//==============================================================================
DLL_API void enable_blending();
DLL_API void disable_blending();
//==============================================================================
DLL_API void blend_func_additive();
DLL_API void blend_func_multiplicative();
DLL_API void blend_func_subtractive();
DLL_API void blend_func_alpha();
//==============================================================================
DLL_API GLint get_total_available_memory();
DLL_API GLint get_current_available_memory();
//==============================================================================
DLL_API GLint bound_texture(GLenum binding);
DLL_API GLint bound_texture1d();
DLL_API GLint bound_texture2d();
DLL_API GLint bound_texture3d();
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
