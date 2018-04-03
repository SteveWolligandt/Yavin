#ifndef __YAVIN_OIT_WINDOW__
#define __YAVIN_OIT_WINDOW__

#include "AtomicCounterBuffer.h"
#include "PixelUnpackBuffer.h"
#include "Shader.h"
#include "ShaderStorageBuffer.h"
#include "Texture2D.h"
#include "VBOHelpers.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Window.h"

namespace Yavin {

class OITWindow : public Window {
 public:
  //============================================================================
  class screen_quad_t {
   public:
    using vbo_t = VertexBuffer<vec2>;
    screen_quad_t()
        : m_vbo{vec2{0, 0}, vec2{1, 0}, vec2{0, 1}, vec2{1, 1}},
          m_ibo{0, 1, 2, 3} {
      m_vao.bind();
      m_vbo.bind();
      m_ibo.bind();
      vbo_t::activate_attributes();
    }

    void draw() {
      m_vao.bind();
      m_vao.draw(TRIANGLE_STRIP, 4);
    }

   private:
    VertexArray m_vao;
    vbo_t       m_vbo;
    IndexBuffer m_ibo;
  };
  //============================================================================
  struct linked_list_element {
    vec4         color;
    unsigned int next_index;
    float        depth;
    vec2         pad;
  };
  //============================================================================
  const static std::string vertex_shader_path;
  const static std::string fragment_shader_path;
  //----------------------------------------------------------------------------
  DLL_API OITWindow(const std::string& name, const int width,
                    const unsigned int height, const unsigned int major = 4,
                    const unsigned int minor = 5);

  //----------------------------------------------------------------------------
  DLL_API void init();
  //----------------------------------------------------------------------------
  void set_render_function(std::function<void()> render_function) {
    m_oit_render_function = render_function;
  }
  //----------------------------------------------------------------------------
  void set_clear_color(const glm::vec4& clear_color) {
    m_linked_list_render_shader.set_uniform("clear_color", clear_color);
  }

 private:
  std::function<void()> m_oit_render_function;

  size_t m_width, m_height;

  Shader                                   m_linked_list_render_shader;
  AtomicCounterBuffer                      m_atomic_counter;
  ShaderStorageBuffer<linked_list_element> m_linked_list;
  Texture2D<unsigned int, R>               m_head_indices_tex;
  PixelUnpackBuffer<unsigned int>          m_clear_buffer;
  screen_quad_t                            m_fullscreen_quad;
};

}  // namespace Yavin
#endif
