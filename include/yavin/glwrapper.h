#ifndef YAVIN_GL_WRAPPER_H
#define YAVIN_GL_WRAPPER_H

#include <array>

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
DLL_API void depth_func_never();
DLL_API void depth_func_less();
DLL_API void depth_func_equal();
DLL_API void depth_func_lequal();
DLL_API void depth_func_greater();
DLL_API void depth_func_notequal();
DLL_API void depth_func_gequal();
DLL_API void depth_func_always();
//==============================================================================
DLL_API void enable_multisampling();
DLL_API void disable_multisampling();
DLL_API bool multisampling_enabled();
//==============================================================================
DLL_API void enable_blending();
DLL_API void disable_blending();
//==============================================================================
DLL_API void enable_face_culling();
DLL_API void disable_face_culling();
DLL_API void set_front_face_culling();
DLL_API void set_back_face_culling();
//==============================================================================
DLL_API void blend_func_additive();
DLL_API void blend_func_multiplicative();
DLL_API void blend_func_subtractive();
DLL_API void blend_func_alpha();
//==============================================================================
DLL_API GLint get_total_available_memory();
DLL_API GLint get_current_available_memory();
//==============================================================================
DLL_API GLuint bound_texture(GLenum binding);
DLL_API GLuint bound_texture1d();
DLL_API GLuint bound_texture2d();
DLL_API GLuint bound_texture3d();
//==============================================================================
DLL_API GLint        max_compute_shared_memory_size();
DLL_API void         barrier();
DLL_API void         shader_storage_barrier();
DLL_API void         shader_image_access_barrier();
DLL_API void         atomic_counter_barrier();
std::array<GLint, 3> max_compute_work_group_count();
std::array<GLint, 3> max_compute_work_group_size();
GLint                max_compute_work_group_invocations();
GLint                max_compute_image_uniforms();
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
