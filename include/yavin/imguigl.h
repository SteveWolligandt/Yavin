#ifndef YAVIN_IMGUIGL_H
#define YAVIN_IMGUIGL_H
//==============================================================================
#include "glincludes.h"
#include <imgui/imgui.h>
#include <iostream>
//==============================================================================
namespace yavin {
//==============================================================================
struct imguigl {
  // OpenGL Data
  GLuint m_gl_version = 0;  // Extracted at runtime using GL_MAJOR_VERSION,
                            // GL_MINOR_VERSION queries.
  char m_glsl_version_string[32] =
      "";  // Specified by user or detected based on compile time GL settings.
  GLuint m_font_texture  = 0;
  GLuint m_shader_handle = 0, m_vert_handle = 0, m_frag_handle = 0;
  int    m_attrib_location_tex   = 0,
      m_attrib_location_proj_mtx = 0;  // Uniforms location
  int m_attrib_location_vtx_pos = 0, m_attrib_location_vtx_uv = 0,
      m_attrib_location_vtx_color = 0;  // Vertex attributes location
  unsigned int m_vbo_handle = 0, m_elements_handle = 0;

  imguigl();

  ~imguigl();

  void new_frame();

  void setup_render_state(ImDrawData* draw_data, int fb_width, int fb_height,
                          GLuint vertex_array_object);

  // OpenGL3 Render function.
  // (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(),
  // but you can now call this directly from your main loop) Note that this
  // implementation is little overcomplicated because we are saving/setting
  // up/restoring every OpenGL state explicitly, in order to be able to run
  // within any OpenGL engine that doesn't do so.
  void render_draw_data(ImDrawData* draw_data);

  bool create_fonts_texture();

  void destroy_fonts_texture();

  // If you get an error please report on github. You may try different GL
  // context version or GLSL version. See GL<>GLSL version table at the top of
  // this file.
  bool check_shader(GLuint handle, const char* desc);

  // If you get an error please report on GitHub. You may try different GL
  // context version or GLSL version.
  bool check_program(GLuint handle, const char* desc);

  bool create_device_objects();

  void destroy_device_objects();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
