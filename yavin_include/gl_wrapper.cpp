#include "gl_wrapper.h"

namespace Yavin {
void clear_color_buffer() {
  glClear(GL_COLOR_BUFFER_BIT);
  gl_error_check("glClear");
}
void clear_depth_Buffer() {
  glClear(GL_DEPTH_BUFFER_BIT);
  gl_error_check("glClear");
}
void clear_color_depth_buffer() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl_error_check("glClear");
}
void set_clear_color(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  gl_error_check("glClearColor");
}
void flush() {
  glFlush();
  gl_error_check("glFlush");
}
void enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
  gl_error_check("glEnable(GL_DEPTH_TEST)");
}
void disable_depth_test() {
  glDisable(GL_DEPTH_TEST);
  gl_error_check("glDisable(GL_DEPTH_TEST)");
}
void enable_depth_write() {
  glDepthMask(GL_TRUE);
  gl_error_check("glDepthMask(GL_TRUE)");
}
void disable_depth_write() {
  glDepthMask(GL_FALSE);
  gl_error_check("glDepthMask(GL_FALSE)");
}
void enable_blending() {
  glEnable(GL_BLEND);
  gl_error_check("glEnable(GL_BLEND)");
}
void disable_blending() {
  glDisable(GL_BLEND);
  gl_error_check("glDisable(GL_BLEND)");
}
void blend_func(GLenum sfactor, GLenum dfactor) {
  glBlendFunc(sfactor, dfactor);
  gl_error_check("glBlendFunc");
}
void blend_func_additive() { blend_func(GL_ONE, GL_ONE); }
void blend_func_multiplicative() { blend_func(GL_ONE, GL_ONE); }
void blend_func_subtractive() { blend_func(GL_ONE, GL_ONE); }
void blend_func_alpha() { blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
void set_viewport(unsigned int left, unsigned int bottom, unsigned int width,
                  unsigned int height) {
  glViewport(left, bottom, width, height);
  gl_error_check("glViewport(" + std::to_string(left) + ", " +
                 std::to_string(bottom) + ", " + std::to_string(width) + ", " +
                 std::to_string(height) + ")");
}
}  // namespace Yavin
