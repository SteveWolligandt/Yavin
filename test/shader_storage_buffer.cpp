#include <yavin>

using namespace Yavin;

const unsigned int screen_width     = 800;
const unsigned int screen_height    = 600;
const unsigned int num_pixels       = screen_width * screen_height;
const unsigned int linked_list_size = num_pixels * 3;

class ShaderStorageTestShader : public Shader {
 public:
  ShaderStorageTestShader(unsigned int screen_width, unsigned int screen_height,
                          unsigned int linked_list_size)
      : Shader() {
    add_shader_stage<VertexShader>("shader_storage_test.vert");
    add_shader_stage<FragmentShader>("shader_storage_test.frag");
    create();
    set_screen_width(screen_width);
    set_screen_height(screen_height);
    set_linked_list_size(linked_list_size);
  }
  void set_screen_width(unsigned int screen_width) {
    set_uniform("screen_width", screen_width);
  }
  void set_screen_height(unsigned int screen_height) {
    set_uniform("screen_height", screen_height);
  }
  void set_linked_list_size(unsigned int size) {
    set_uniform("linked_list_size", size);
  }
  void set_color(const glm::vec4 color) { set_uniform("color", color); }
};

class LinkedListRenderShader : public Shader {
 public:
  LinkedListRenderShader() : Shader() {
    add_shader_stage<VertexShader>("linked_list_render.vert");
    add_shader_stage<FragmentShader>("linked_list_render.frag");
    create();
  }
};

class TriangleStrip {
 public:
  using vbo_t = VertexBuffer<vec3>;
  TriangleStrip(std::initializer_list<vbo_t::data_t>&& vertices)
      : size(vertices.size()), vbo(std::move(vertices)) {
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < size; ++i) ibo.push_back(i);
    ibo.upload_data();

    vao.bind();
    vbo.bind();
    ibo.bind();
    vbo_t::activate_attributes();
  }

  void draw() {
    vao.bind();
    vao.draw(Yavin::TRIANGLE_STRIP, size);
  }

 private:
  size_t      size;
  VertexArray vao;
  vbo_t       vbo;
  IndexBuffer ibo;
};

struct linked_list_element {
  vec4         color;
  unsigned int next_index;
  float        depth;
  vec2         pad;
};

using head_tex_t = Texture2D<unsigned int, R>;

int main() {
  Window w("Shader Storage Buffer", screen_width, screen_height, 4, 5);
  set_clear_color(1, 1, 1, 1);

  // indices to last elements of linked list are being saved here
  head_tex_t head_indices_tex(screen_width, screen_height);
  head_indices_tex.bind();
  head_indices_tex.set_interpolation_mode(Texture::NEAREST);
  head_indices_tex.set_wrap_mode(Texture::REPEAT);
  head_indices_tex.unbind();
  glBindImageTexture(0, head_indices_tex.id(), 0, GL_FALSE, 0, GL_READ_WRITE,
                     GL_R32UI);

  // used or clearing head_indices_tex
  PixelUnpackBuffer<unsigned int> clear_buffer;
  for (size_t i = 0; i < num_pixels; ++i) clear_buffer.push_back(0xffffffff);
  clear_buffer.upload_data();

  // for index counting in shader
  AtomicCounterBuffer atomic_counter{0};
  atomic_counter.bind(0);
  // linked list shader
  ShaderStorageTestShader shader(screen_width, screen_height, linked_list_size);
  LinkedListRenderShader  linked_list_render_shader;
  // linked list
  ShaderStorageBuffer<linked_list_element> linked_list(linked_list_size);
  linked_list.bind(0);

  //--------------
  // scene objects
  OrthographicCamera ortho_cam(0, 1, 0, 1, -1, 1, screen_width, screen_height);
  PerspectiveCamera  cam(30, (float)screen_width / (float)screen_height, 0.01,
                        100, screen_width, screen_height);
  cam.transform().translate(0, 0, 3);
  TriangleStrip quad1{vec3{-1, -1, 0}, vec3{1, -1, 2}, vec3{-1, 1, 0},
                      vec3{1, 1, 2}};
  TriangleStrip quad2{vec3{-1, -1.4, 1}, vec3{1, -1.4, 1}, vec3{-1, 0.9, 1},
                      vec3{1, 0.9, 1}};
  TriangleStrip fullscreen_quad{vec3{0, 0, 0}, vec3{1, 0, 0}, vec3{0, 1, 0},
                                vec3{1, 1, 0}};
  w.set_render_function([&]() {
    //--------------------------------------------------------------------------
    clear_color_buffer();

    // clear linked list
    head_indices_tex.bind();
    clear_buffer.bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screen_width, screen_height,
                    head_tex_t::format, head_tex_t::type, 0);
    clear_buffer.unbind();
    head_indices_tex.unbind();
    atomic_counter.to_zero();

    // render scene
    shader.bind();
    shader.set_uniform("projection", cam.projection_matrix());
    shader.set_uniform("modelview", cam.view_matrix());

    shader.set_color({1, 0, 0, 0.5});
    quad1.draw();

    shader.set_color({0, 1, 0, 0.5});
    quad2.draw();

    // draw linked list
    linked_list_render_shader.bind();
    linked_list_render_shader.set_uniform("projection",
                                          ortho_cam.projection_matrix());
    linked_list_render_shader.set_uniform("modelview", ortho_cam.view_matrix());
    fullscreen_quad.draw();

    // finish window
    w.swap_buffers();
    w.poll_events();
    //--------------------------------------------------------------------------
  });
  w.start_rendering();
}
