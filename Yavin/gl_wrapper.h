#ifndef __YAVIN_GL_WRAPPER_H__
#define __YAVIN_GL_WRAPPER_H__

#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {

void clear_color_buffer();
void clear_depth_Buffer();
void clear_color_depth_duffer();
void set_clear_Color(float r, float g, float b, float a);
void flush();
void enable_depth_test();
void disable_depth_test();
void enable_depth_write();
void disable_depth_write();
void enable_blending();
void disable_blending();
void blend_func(GLenum sfactor, GLenum dfactor);
void blend_func_additive();
void blend_func_multiplicative();
void blend_func_subtractive();
void blend_func_alpha();
void set_viewport(unsigned int left, unsigned int bottom, unsigned int width, unsigned int height);
}  // namespace Yavin

#endif