#include "gl_wrapper.h"

//==============================================================================
namespace yavin {
//==============================================================================

void clear_color_buffer() { gl::clear(GL_COLOR_BUFFER_BIT); }
void clear_depth_Buffer() { gl::clear(GL_DEPTH_BUFFER_BIT); }
void clear_color_depth_buffer() {
  gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void enable_depth_test() { gl::enable(GL_DEPTH_TEST); }
void disable_depth_test() { gl::disable(GL_DEPTH_TEST); }

void enable_depth_write() { gl::depth_mask(GL_TRUE); }
void disable_depth_write() { gl::depth_mask(GL_FALSE); }

void enable_blending() { gl::enable(GL_BLEND); }
void disable_blending() { gl::disable(GL_BLEND); }

void blend_func_additive() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_multiplicative() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_subtractive() { gl::blend_func(GL_ONE, GL_ONE); }
void blend_func_alpha() {
  gl::blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
