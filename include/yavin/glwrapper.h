#ifndef YAVIN_GL_WRAPPER_H
#define YAVIN_GL_WRAPPER_H
//==============================================================================
#include <array>

#include "dllexport.h"
#include "errorcheck.h"
#include "glincludes.h"
//==============================================================================
namespace yavin {
//==============================================================================
DLL_API auto clear_color_buffer() -> void;
DLL_API auto clear_depth_buffer() -> void;
DLL_API auto clear_color_depth_buffer() -> void;
//==============================================================================
DLL_API auto enable_depth_test() -> void;
DLL_API auto disable_depth_test() -> void;
//==============================================================================
DLL_API auto enable_depth_write() -> void;
DLL_API auto disable_depth_write() -> void;
//==============================================================================
DLL_API auto depth_func_never() -> void;
DLL_API auto depth_func_less() -> void;
DLL_API auto depth_func_equal() -> void;
DLL_API auto depth_func_lequal() -> void;
DLL_API auto depth_func_greater() -> void;
DLL_API auto depth_func_notequal() -> void;
DLL_API auto depth_func_gequal() -> void;
DLL_API auto depth_func_always() -> void;
//==============================================================================
DLL_API auto enable_multisampling() -> void;
DLL_API auto disable_multisampling() -> void;
DLL_API auto multisampling_enabled() -> bool;
//==============================================================================
DLL_API auto enable_blending() -> void;
DLL_API auto disable_blending() -> void;
//==============================================================================
DLL_API auto enable_face_culling() -> void;
DLL_API auto disable_face_culling() -> void;
DLL_API auto set_front_face_culling() -> void;
DLL_API auto set_back_face_culling() -> void;
//==============================================================================
DLL_API auto blend_func_additive() -> void;
DLL_API auto blend_func_multiplicative() -> void;
DLL_API auto blend_func_subtractive() -> void;
DLL_API auto blend_func_alpha() -> void;
//==============================================================================
DLL_API auto get_total_available_memory() -> GLint;
DLL_API auto get_current_available_memory() -> GLint;
//==============================================================================
DLL_API auto current_program() -> GLuint;
DLL_API auto bound_texture(GLenum binding) -> GLuint;
DLL_API auto bound_texture1d() -> GLuint;
DLL_API auto bound_texture2d() -> GLuint;
DLL_API auto bound_texture3d() -> GLuint;
//==============================================================================
DLL_API auto max_compute_shared_memory_size() -> GLint;
DLL_API auto barrier() -> void;
DLL_API auto shader_storage_barrier() -> void;
DLL_API auto shader_image_access_barrier() -> void;
DLL_API auto atomic_counter_barrier() -> void;
DLL_API auto max_compute_work_group_count() -> std::array<GLint, 3>;
DLL_API auto max_compute_work_group_size() -> std::array<GLint, 3>;
DLL_API auto max_compute_work_group_invocations() -> GLint;
DLL_API auto max_compute_image_uniforms() -> GLint;
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
