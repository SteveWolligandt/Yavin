#include <yavin/glwrapper.h>
#include <yavin/glfunctions.h>
//==============================================================================
namespace yavin {
//==============================================================================
void clear_color_buffer() {
  gl::clear(GL_COLOR_BUFFER_BIT);
}
void clear_depth_buffer() {
  gl::clear(GL_DEPTH_BUFFER_BIT);
}
void clear_color_depth_buffer() {
  gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//==============================================================================
void enable_depth_test() {
  gl::enable(GL_DEPTH_TEST);
}
void disable_depth_test() {
  gl::disable(GL_DEPTH_TEST);
}
//==============================================================================
void enable_depth_write() {
  gl::depth_mask(GL_TRUE);
}
void disable_depth_write() {
  gl::depth_mask(GL_FALSE);
}
//==============================================================================
void depth_func_never() {
  gl::depth_func(GL_NEVER);
}
void depth_func_less() {
  gl::depth_func(GL_LESS);
}
void depth_func_equal() {
  gl::depth_func(GL_EQUAL);
}
void depth_func_lequal() {
  gl::depth_func(GL_LEQUAL);
}
void depth_func_greater() {
  gl::depth_func(GL_GREATER);
}
void depth_func_notequal() {
  gl::depth_func(GL_NOTEQUAL);
}
void depth_func_gequal() {
  gl::depth_func(GL_GEQUAL);
}
void depth_func_always() {
  gl::depth_func(GL_ALWAYS);
}
//==============================================================================
void enable_blending() {
  gl::enable(GL_BLEND);
}
void disable_blending() {
  gl::disable(GL_BLEND);
}
//==============================================================================
void enable_face_culling() {
  gl::enable(GL_CULL_FACE);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void disable_face_culling() {
  gl::disable(GL_CULL_FACE);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void set_front_face_culling() {
  gl::cull_face(GL_FRONT);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void set_back_face_culling() {
  gl::cull_face(GL_BACK);
}
//==============================================================================
void enable_multisampling() {
  gl::enable(GL_MULTISAMPLE);
}
void disable_multisampling() {
  gl::disable(GL_MULTISAMPLE);
}
bool multisampling_enabled() {
  return gl::is_enabled(GL_MULTISAMPLE);
}
//==============================================================================
void blend_func_additive() {
  gl::blend_func(GL_ONE, GL_ONE);
}
void blend_func_multiplicative() {
  gl::blend_func(GL_ONE, GL_ONE);
}
void blend_func_subtractive() {
  gl::blend_func(GL_ONE, GL_ONE);
}
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
GLuint bound_texture(GLenum binding) {
  GLint tex;
  gl::get_integer_v(binding, &tex);
  return static_cast<GLuint>(tex);
}
//------------------------------------------------------------------------------
GLuint bound_texture1d() {
  return bound_texture(GL_TEXTURE_BINDING_1D);
}
//------------------------------------------------------------------------------
GLuint bound_texture2d() {
  return bound_texture(GL_TEXTURE_BINDING_2D);
}
//------------------------------------------------------------------------------
GLuint bound_texture3d() {
  return bound_texture(GL_TEXTURE_BINDING_3D);
}
//==============================================================================
GLint max_compute_shared_memory_size() {
  GLint s;
  gl::get_integer_v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &s);
  return s;
}
//------------------------------------------------------------------------------
void barrier() {
  gl::memory_barrier(
      GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_ELEMENT_ARRAY_BARRIER_BIT |
      GL_UNIFORM_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT |
      GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_COMMAND_BARRIER_BIT |
      GL_PIXEL_BUFFER_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT |
      GL_BUFFER_UPDATE_BARRIER_BIT | GL_FRAMEBUFFER_BARRIER_BIT |
      GL_TRANSFORM_FEEDBACK_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT |
      GL_SHADER_STORAGE_BARRIER_BIT);
}
//------------------------------------------------------------------------------
void shader_storage_barrier() {
  gl::memory_barrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
//------------------------------------------------------------------------------
void shader_image_access_barrier() {
  gl::memory_barrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
//------------------------------------------------------------------------------
void atomic_counter_barrier() {
  gl::memory_barrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
}
//------------------------------------------------------------------------------
std::array<GLint, 3> max_compute_work_group_count() {
  std::array<GLint, 3> work_grp_cnt;
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
  return work_grp_cnt;
}
//------------------------------------------------------------------------------
std::array<GLint, 3> max_compute_work_group_size() {
  std::array<GLint, 3> work_grp_size;
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
  gl::get_integeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
  return work_grp_size;
}
//------------------------------------------------------------------------------
GLint max_compute_work_group_invocations() {
  GLint val;
  gl::get_integer_v(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &val);
  return val;
}
//------------------------------------------------------------------------------
GLint max_compute_image_uniforms() {
  GLint val;
  gl::get_integer_v(GL_MAX_COMPUTE_IMAGE_UNIFORMS, &val);
  return val;
}

//==============================================================================
}  // namespace yavin
//==============================================================================
