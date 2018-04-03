#include "OITWindow.h"
#include "OrthographicCamera.h"
#include "gl_wrapper.h"
#include "shader_include_paths.h"

namespace Yavin {

const std::string OITWindow::vertex_shader_path =
    shader_dir + std::string("linked_list_render.vert");
const std::string OITWindow::fragment_shader_path =
    shader_dir + std::string("linked_list_render.frag");
OITWindow::OITWindow(const std::string& name, const int width,
                     const unsigned int height, const unsigned int major,
                     const unsigned int minor)
    : Window(name, width, height, major, minor),
      m_oit_render_function([]() {}),
      m_width(width),
      m_height(height),
      m_atomic_counter{0},
      m_linked_list(width * height),
      m_head_indices_tex(width, height) {
  OrthographicCamera cam(0, 1, 0, 1, -1, 1, width, height);
  m_linked_list_render_shader.add_shader_stage<VertexShader>(
      vertex_shader_path);
  m_linked_list_render_shader.add_shader_stage<FragmentShader>(
      fragment_shader_path);
  m_linked_list_render_shader.create();
  m_linked_list_render_shader.bind();
  m_linked_list_render_shader.set_uniform("projection",
                                          cam.projection_matrix());
  m_linked_list_render_shader.set_uniform("modelview", cam.view_matrix());

  m_atomic_counter.bind(5);
  m_linked_list.bind(80);

  m_head_indices_tex.bind();
  m_head_indices_tex.set_interpolation_mode(Texture::NEAREST);
  m_head_indices_tex.set_wrap_mode(Texture::REPEAT);
  m_head_indices_tex.unbind();
  m_head_indices_tex.bind_image_texture(7);

  for (size_t i = 0; i < m_width * m_height; ++i)
    m_clear_buffer.push_back(0xffffffff);
  m_clear_buffer.upload_data();

  Window::set_render_function([this]() {
    clear_color_buffer();

    // clear linked list
    m_atomic_counter.to_zero();
    m_head_indices_tex.set_data(m_clear_buffer);

    // user render function
    m_oit_render_function();

    // draw linked list
    m_linked_list_render_shader.bind();
    m_fullscreen_quad.draw();

    // finish window
    swap_buffers();
    poll_events();
  });
}

void OITWindow::set_render_function(std::function<void()> render_function) {
  m_oit_render_function = render_function;
}
}  // namespace Yavin
