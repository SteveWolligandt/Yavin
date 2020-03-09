#include <yavin/imguigl.h>
#include <iostream>
//==============================================================================
namespace yavin {
//==============================================================================
imguigl::imguigl() {
  // Query for GL version
  GLint major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  m_gl_version = major * 1000 + minor;

  // Setup back-end capabilities flags
  ImGuiIO& io            = ImGui::GetIO();
  io.BackendRendererName = "yavin";

  if (m_gl_version >= 3200) {
    io.BackendFlags |=
        ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the
                                                 // ImDrawCmd::VtxOffset field,
                                                 // allowing for large meshes.
  }

  // Store GLSL version string so we can refer to it later in case we recreate
  // shaders. Note: GLSL version is NOT the same as GL version. Leave this to
  // NULL if unsure.
  const char* glsl_version = "#version 330";
  IM_ASSERT((int)strlen(glsl_version) + 2 <
            IM_ARRAYSIZE(m_glsl_version_string));
  strcpy(m_glsl_version_string, glsl_version);
  strcat(m_glsl_version_string, "\n");
}
//------------------------------------------------------------------------------
imguigl::~imguigl() {
  destroy_device_objects();
}
//------------------------------------------------------------------------------
void imguigl::new_frame() {
  if (!m_shader_handle) { create_device_objects(); }
}
//------------------------------------------------------------------------------
void imguigl::setup_render_state(ImDrawData* draw_data, int fb_width,
                                 int fb_height, GLuint vertex_array_object) {
  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled, polygon fill
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

  // Setup viewport, orthographic projection matrix
  // Our visible imgui space lies from draw_data->DisplayPos (top left) to
  // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos
  // is (0,0) for single viewport apps.
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  float       L = draw_data->DisplayPos.x;
  float       R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
  float       T = draw_data->DisplayPos.y;
  float       B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
  const float ortho_projection[4][4] = {
      {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
      {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
      {0.0f, 0.0f, -1.0f, 0.0f},
      {(R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f},
  };
  glUseProgram(m_shader_handle);
  glUniform1i(m_attrib_location_tex, 0);
  glUniformMatrix4fv(m_attrib_location_proj_mtx, 1, GL_FALSE,
                     &ortho_projection[0][0]);
#ifdef GL_SAMPLER_BINDING
  glBindSampler(0, 0);  // We use combined texture/sampler state. Applications
                        // using GL 3.3 may set that otherwise.
#endif

  (void)vertex_array_object;
#ifndef IMGUI_IMPL_OPENGL_ES2
  glBindVertexArray(vertex_array_object);
#endif

  // Bind vertex/index buffers and setup attributes for ImDrawVert
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elements_handle);
  glEnableVertexAttribArray(m_attrib_location_vtx_pos);
  glEnableVertexAttribArray(m_attrib_location_vtx_uv);
  glEnableVertexAttribArray(m_attrib_location_vtx_color);
  glVertexAttribPointer(m_attrib_location_vtx_pos, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert),
                        (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(m_attrib_location_vtx_uv, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert),
                        (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(m_attrib_location_vtx_color, 4, GL_UNSIGNED_BYTE,
                        GL_TRUE, sizeof(ImDrawVert),
                        (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}
//------------------------------------------------------------------------------
// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(),
// but you can now call this directly from your main loop) Note that this
// implementation is little overcomplicated because we are saving/setting
// up/restoring every OpenGL state explicitly, in order to be able to run
// within any OpenGL engine that doesn't do so.
void imguigl::render_draw_data(ImDrawData* draw_data) {
  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  int fb_width =
      (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
  int fb_height =
      (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
  if (fb_width <= 0 || fb_height <= 0) return;

  // Backup GL state
  GLenum last_active_texture;
  glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
  glActiveTexture(GL_TEXTURE0);
  GLint last_program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
#ifdef GL_SAMPLER_BINDING
  GLint last_sampler;
  glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
#endif
  GLint last_array_buffer;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
  GLint last_vertex_array_object;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
#endif
#ifdef GL_POLYGON_MODE
  GLint last_polygon_mode[2];
  glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
#endif
  GLint last_viewport[4];
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLint last_scissor_box[4];
  glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
  GLenum last_blend_src_rgb;
  glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
  GLenum last_blend_dst_rgb;
  glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
  GLenum last_blend_src_alpha;
  glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
  GLenum last_blend_dst_alpha;
  glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
  GLenum last_blend_equation_rgb;
  glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
  GLenum last_blend_equation_alpha;
  glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
  GLboolean last_enable_blend        = glIsEnabled(GL_BLEND);
  GLboolean last_enable_cull_face    = glIsEnabled(GL_CULL_FACE);
  GLboolean last_enable_depth_test   = glIsEnabled(GL_DEPTH_TEST);
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
  bool      clip_origin_lower_left   = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
  GLenum last_clip_origin = 0;
  glGetIntegerv(GL_CLIP_ORIGIN,
                (GLint*)&last_clip_origin);  // Support for GL 4.5's
                                             // glClipControl(GL_UPPER_LEFT)
  if (last_clip_origin == GL_UPPER_LEFT) clip_origin_lower_left = false;
#endif

  // Setup desired GL state
  // Recreate the VAO every time (this is to easily allow multiple GL contexts
  // to be rendered to. VAO are not shared among GL contexts) The renderer
  // would actually work without any VAO bound, but then our VertexAttrib
  // calls would overwrite the default one currently bound.
  GLuint vertex_array_object = 0;
#ifndef IMGUI_IMPL_OPENGL_ES2
  glGenVertexArrays(1, &vertex_array_object);
#endif
  setup_render_state(draw_data, fb_width, fb_height, vertex_array_object);

  // Will project scissor/clipping rectangles into framebuffer space
  ImVec2 clip_off =
      draw_data->DisplayPos;  // (0,0) unless using multi-viewports
  ImVec2 clip_scale =
      draw_data->FramebufferScale;  // (1,1) unless using retina display which
                                    // are often (2,2)

  // Render command lists
  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];

    // Upload vertex/index buffers
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                 (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
                 (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback != NULL) {
        // User callback, registered via ImDrawList::AddCallback()
        // (ImDrawCallback_ResetRenderState is a special callback value used
        // by the user to request the renderer to reset render state.)
        if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
          setup_render_state(draw_data, fb_width, fb_height,
                             vertex_array_object);
        else
          pcmd->UserCallback(cmd_list, pcmd);
      } else {
        // Project scissor/clipping rectangles into framebuffer space
        ImVec4 clip_rect;
        clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
        clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
        clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
        clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

        if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
            clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
          // Apply scissor/clipping rectangle
          if (clip_origin_lower_left)
            glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w),
                      (int)(clip_rect.z - clip_rect.x),
                      (int)(clip_rect.w - clip_rect.y));
          else
            glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z,
                      (int)clip_rect.w);  // Support for GL 4.5 rarely used
                                          // glClipControl(GL_UPPER_LEFT)

          // Bind texture, Draw
          glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
#if IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
          if (m_gl_version >= 3200)
            glDrawElementsBaseVertex(
                GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)),
                (GLint)pcmd->VtxOffset);
          else
#endif
            glDrawElements(
                GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
                sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
                (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
        }
      }
    }
  }

  // Destroy the temporary VAO
#ifndef IMGUI_IMPL_OPENGL_ES2
  glDeleteVertexArrays(1, &vertex_array_object);
#endif

  // Restore modified GL state
  glUseProgram(last_program);
  glBindTexture(GL_TEXTURE_2D, last_texture);
#ifdef GL_SAMPLER_BINDING
  glBindSampler(0, last_sampler);
#endif
  glActiveTexture(last_active_texture);
#ifndef IMGUI_IMPL_OPENGL_ES2
  glBindVertexArray(last_vertex_array_object);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb,
                      last_blend_src_alpha, last_blend_dst_alpha);
  if (last_enable_blend)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);
  if (last_enable_cull_face)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
  if (last_enable_depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
  if (last_enable_scissor_test)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
  glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
#endif
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
             (GLsizei)last_viewport[3]);
  glScissor(last_scissor_box[0], last_scissor_box[1],
            (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}
//------------------------------------------------------------------------------
bool imguigl::create_fonts_texture() {
  // Build texture atlas
  ImGuiIO&       io = ImGui::GetIO();
  unsigned char* pixels;
  int            width, height;
  io.Fonts->GetTexDataAsRGBA32(
      &pixels, &width,
      &height);  // Load as RGBA 32-bit (75% of the memory is wasted, but
                 // default font is so small) because it is more likely to be
                 // compatible with user's existing shaders. If your
                 // ImTextureId represent a higher-level concept than just a
                 // GL texture id, consider calling GetTexDataAsAlpha8()
                 // instead to save on GPU memory.

  // Upload texture to graphics system
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGenTextures(1, &m_font_texture);
  glBindTexture(GL_TEXTURE_2D, m_font_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (ImTextureID)(intptr_t)m_font_texture;

  // Restore state
  glBindTexture(GL_TEXTURE_2D, last_texture);

  return true;
}
//------------------------------------------------------------------------------
void imguigl::destroy_fonts_texture() {
  if (m_font_texture) {
    ImGuiIO& io = ImGui::GetIO();
    glDeleteTextures(1, &m_font_texture);
    io.Fonts->TexID = 0;
    m_font_texture  = 0;
  }
}
//------------------------------------------------------------------------------
// If you get an error please report on github. You may try different GL
// context version or GLSL version. See GL<>GLSL version table at the top of
// this file.
bool imguigl::check_shader(GLuint handle, const char* desc) {
  GLint status = 0, log_length = 0;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
  if ((GLboolean)status == GL_FALSE)
    std::cerr << "ERROR: imguigl::create_device_objects: failed to compile "
              << desc << "!\n";
  if (log_length > 1) {
    ImVector<char> buf;
    buf.resize((int)(log_length + 1));
    glGetShaderInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
    std::cerr << buf.begin() << '\n';
  }
  return (GLboolean)status == GL_TRUE;
}
//------------------------------------------------------------------------------
// If you get an error please report on GitHub. You may try different GL
// context version or GLSL version.
bool imguigl::check_program(GLuint handle, const char* desc) {
  GLint status = 0, log_length = 0;
  glGetProgramiv(handle, GL_LINK_STATUS, &status);
  glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
  if ((GLboolean)status == GL_FALSE)
    std::cerr << "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to link "
              << desc
              << "! "
                 "(with GLSL \'"
              << m_glsl_version_string << "\')\n";

  if (log_length > 1) {
    ImVector<char> buf;
    buf.resize((int)(log_length + 1));
    glGetProgramInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
    std::cerr << buf.begin() << '\n';
  }
  return (GLboolean)status == GL_TRUE;
}
//------------------------------------------------------------------------------
bool imguigl::create_device_objects() {
  // Backup GL state
  GLint last_texture, last_array_buffer;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
  GLint last_vertex_array;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
#endif

  // Parse GLSL version string
  int glsl_version = 130;
  sscanf(m_glsl_version_string, "#version %d", &glsl_version);

  const GLchar* vertex_shader_glsl_120 =
      "uniform mat4 ProjMtx;\n"
      "attribute vec2 Position;\n"
      "attribute vec2 UV;\n"
      "attribute vec4 Color;\n"
      "varying vec2 Frag_UV;\n"
      "varying vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    Frag_UV = UV;\n"
      "    Frag_Color = Color;\n"
      "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* vertex_shader_glsl_130 =
      "uniform mat4 ProjMtx;\n"
      "in vec2 Position;\n"
      "in vec2 UV;\n"
      "in vec4 Color;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    Frag_UV = UV;\n"
      "    Frag_Color = Color;\n"
      "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* vertex_shader_glsl_300_es =
      "precision mediump float;\n"
      "layout (location = 0) in vec2 Position;\n"
      "layout (location = 1) in vec2 UV;\n"
      "layout (location = 2) in vec4 Color;\n"
      "uniform mat4 ProjMtx;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    Frag_UV = UV;\n"
      "    Frag_Color = Color;\n"
      "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* vertex_shader_glsl_410_core =
      "layout (location = 0) in vec2 Position;\n"
      "layout (location = 1) in vec2 UV;\n"
      "layout (location = 2) in vec4 Color;\n"
      "uniform mat4 ProjMtx;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    Frag_UV = UV;\n"
      "    Frag_Color = Color;\n"
      "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* fragment_shader_glsl_120 =
      "#ifdef GL_ES\n"
      "    precision mediump float;\n"
      "#endif\n"
      "uniform sampler2D Texture;\n"
      "varying vec2 Frag_UV;\n"
      "varying vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
      "}\n";

  const GLchar* fragment_shader_glsl_130 =
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
      "}\n";

  const GLchar* fragment_shader_glsl_300_es =
      "precision mediump float;\n"
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "layout (location = 0) out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
      "}\n";

  const GLchar* fragment_shader_glsl_410_core =
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "uniform sampler2D Texture;\n"
      "layout (location = 0) out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
      "}\n";

  // Select shaders matching our GLSL versions
  const GLchar* vertex_shader   = NULL;
  const GLchar* fragment_shader = NULL;
  if (glsl_version < 130) {
    vertex_shader   = vertex_shader_glsl_120;
    fragment_shader = fragment_shader_glsl_120;
  } else if (glsl_version >= 410) {
    vertex_shader   = vertex_shader_glsl_410_core;
    fragment_shader = fragment_shader_glsl_410_core;
  } else if (glsl_version == 300) {
    vertex_shader   = vertex_shader_glsl_300_es;
    fragment_shader = fragment_shader_glsl_300_es;
  } else {
    vertex_shader   = vertex_shader_glsl_130;
    fragment_shader = fragment_shader_glsl_130;
  }

  // Create shaders
  const GLchar* vertex_shader_with_version[2] = {m_glsl_version_string,
                                                 vertex_shader};
  m_vert_handle = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_vert_handle, 2, vertex_shader_with_version, NULL);
  glCompileShader(m_vert_handle);
  check_shader(m_vert_handle, "vertex shader");

  const GLchar* fragment_shader_with_version[2] = {m_glsl_version_string,
                                                   fragment_shader};
  m_frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_frag_handle, 2, fragment_shader_with_version, NULL);
  glCompileShader(m_frag_handle);
  check_shader(m_frag_handle, "fragment shader");

  m_shader_handle = glCreateProgram();
  glAttachShader(m_shader_handle, m_vert_handle);
  glAttachShader(m_shader_handle, m_frag_handle);
  glLinkProgram(m_shader_handle);
  check_program(m_shader_handle, "shader program");

  m_attrib_location_tex = glGetUniformLocation(m_shader_handle, "Texture");
  m_attrib_location_proj_mtx =
      glGetUniformLocation(m_shader_handle, "ProjMtx");
  m_attrib_location_vtx_pos = glGetAttribLocation(m_shader_handle, "Position");
  m_attrib_location_vtx_uv  = glGetAttribLocation(m_shader_handle, "UV");
  m_attrib_location_vtx_color = glGetAttribLocation(m_shader_handle, "Color");

  // Create buffers
  glGenBuffers(1, &m_vbo_handle);
  glGenBuffers(1, &m_elements_handle);

  create_fonts_texture();

  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
#ifndef IMGUI_IMPL_OPENGL_ES2
  glBindVertexArray(last_vertex_array);
#endif

  return true;
}
//------------------------------------------------------------------------------
void imguigl::destroy_device_objects() {
  if (m_vbo_handle) {
    glDeleteBuffers(1, &m_vbo_handle);
    m_vbo_handle = 0;
  }
  if (m_elements_handle) {
    glDeleteBuffers(1, &m_elements_handle);
    m_elements_handle = 0;
  }
  if (m_shader_handle && m_vert_handle) {
    glDetachShader(m_shader_handle, m_vert_handle);
  }
  if (m_shader_handle && m_frag_handle) {
    glDetachShader(m_shader_handle, m_frag_handle);
  }
  if (m_vert_handle) {
    glDeleteShader(m_vert_handle);
    m_vert_handle = 0;
  }
  if (m_frag_handle) {
    glDeleteShader(m_frag_handle);
    m_frag_handle = 0;
  }
  if (m_shader_handle) {
    glDeleteProgram(m_shader_handle);
    m_shader_handle = 0;
  }

  destroy_fonts_texture();
}
//==============================================================================
}  // namespace yavin
//==============================================================================
