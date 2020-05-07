#include <yavin/imgui_render_backend.h>
#include <iostream>
//==============================================================================
namespace yavin {
//==============================================================================
imgui_render_backend::imgui_render_backend() {
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
  const char* glsl_version = "#version 410";
  IM_ASSERT((int)strlen(glsl_version) + 2 <
            IM_ARRAYSIZE(m_glsl_version_string));
  strcpy(m_glsl_version_string, glsl_version);
  strcat(m_glsl_version_string, "\n");
   create_device_objects();
}
//------------------------------------------------------------------------------
void imgui_render_backend::setup_render_state(ImDrawData* draw_data, int fb_width,
                                 int fb_height, vertexarray& vao) {
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
  const mat4  ortho_projection = {
      {2.0f / (R - L), 0.0f, 0.0f, (R + L) / (L - R)},
      {0.0f, 2.0f / (T - B), 0.0f, (T + B) / (B - T)},
      {0.0f, 0.0f, -1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  m_shader.bind();
  m_shader.set_texture_slot(0);
  m_shader.set_projection_matrix(ortho_projection);
#ifdef GL_SAMPLER_BINDING
  glBindSampler(0, 0);  // We use combined texture/sampler state. Applications
                        // using GL 3.3 may set that otherwise.
#endif


  vao.bind();
  m_vbo.bind();
  m_ibo.bind();
  m_vbo.activate_attributes(GL_FALSE, GL_FALSE, GL_TRUE);
}
//------------------------------------------------------------------------------
void imgui_render_backend::render_draw_data(ImDrawData* draw_data) {
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
  GLint last_vertex_array_object;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);
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
  vertexarray vao;
  setup_render_state(draw_data, fb_width, fb_height, vao);

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
                             vao);
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
bool imgui_render_backend::create_fonts_texture() {
  // Build texture atlas
  ImGuiIO&      io = ImGui::GetIO();
  std::uint8_t* pixels;
  int           width, height;
  io.Fonts->GetTexDataAsRGBA32(
      &pixels, &width,
      &height);  // Load as RGBA 32-bit (75% of the memory is wasted, but
                 // default font is so small) because it is more likely to be
                 // compatible with user's existing shaders. If your
                 // ImTextureId represent a higher-level concept than just a
                 // GL texture id, consider calling GetTexDataAsAlpha8()
                 // instead to save on GPU memory.

  const auto err = gl::get_error();
  if (err != GL_NO_ERROR) {
    const auto err_str = gl_error_to_string(err);
    throw gl_error(std::string{"imgui"}, err_str);
  }
  // Upload texture to graphics system
  m_font_texture.upload_data(pixels, width, height);
  // Store our identifier
  io.Fonts->TexID = (ImTextureID)(intptr_t)m_font_texture.id();
  return true;
}
//------------------------------------------------------------------------------
bool imgui_render_backend::check_shader(GLuint handle, const char* desc) {
  GLint status = 0, log_length = 0;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
  if ((GLboolean)status == GL_FALSE)
    std::cerr << "ERROR: imgui_render_backend::create_device_objects: failed to compile "
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
bool imgui_render_backend::check_program(GLuint handle, const char* desc) {
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
bool imgui_render_backend::create_device_objects() {
  // Backup GL state
  GLint last_texture, last_array_buffer;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  GLint last_vertex_array;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);




  // Create buffers
  create_fonts_texture();

  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindVertexArray(last_vertex_array);

  return true;
}
//==============================================================================
}  // namespace yavin
//==============================================================================
