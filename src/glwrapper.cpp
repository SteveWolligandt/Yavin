#include <yavin/glwrapper.h>
//==============================================================================
namespace yavin {
//==============================================================================
void clear_color_buffer() { gl::clear(GL_COLOR_BUFFER_BIT); }
void clear_depth_buffer() { gl::clear(GL_DEPTH_BUFFER_BIT); }
void clear_color_depth_buffer() {
  gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//==============================================================================
void enable_depth_test() { gl::enable(GL_DEPTH_TEST); }
void disable_depth_test() { gl::disable(GL_DEPTH_TEST); }
//==============================================================================
void enable_depth_write() { gl::depth_mask(GL_TRUE); }
void disable_depth_write() { gl::depth_mask(GL_FALSE); }
//==============================================================================
void enable_blending() { gl::enable(GL_BLEND); }
void disable_blending() { gl::disable(GL_BLEND); }
//==============================================================================
void enable_multisampling() { gl::enable(GL_MULTISAMPLE); }
void disable_multisampling() { gl::disable(GL_MULTISAMPLE); }
bool multisampling_enabled() { return gl::is_enabled(GL_MULTISAMPLE); }
//==============================================================================
void blend_func_additive() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_multiplicative() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_subtractive() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_alpha() {
  gl::blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//==============================================================================
GLint get_total_available_memory() {
  constexpr auto GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX = 0x9048;
  GLint          tam;
  gl::get_integer_v(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &tam);
  return tam;
}
//------------------------------------------------------------------------------
GLint get_current_available_memory() {
  constexpr auto GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX = 0x9049;
  GLint          tam;
  gl::get_integer_v(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &tam);
  return tam;
}
//==============================================================================
GLint bound_texture(GLenum binding) {
  GLint tex;
  gl::get_integer_v(binding, &tex);
  return tex;
}
//------------------------------------------------------------------------------
GLint bound_texture1d() {
  return bound_texture(GL_TEXTURE_BINDING_1D);
}
//------------------------------------------------------------------------------
GLint bound_texture2d() {
  return bound_texture(GL_TEXTURE_BINDING_2D);
}
//------------------------------------------------------------------------------
GLint bound_texture3d() {
  return bound_texture(GL_TEXTURE_BINDING_3D);
}
//==============================================================================
}  // namespace yavin
//==============================================================================
