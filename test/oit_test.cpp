#include <yavin>

using namespace Yavin;

const unsigned int screen_width     = 800;
const unsigned int screen_height    = 600;
const unsigned int num_pixels       = screen_width * screen_height;
const unsigned int linked_list_size = num_pixels * 3;

//==============================================================================
class ColorLinkedListShader : public Shader {
 public:
  ColorLinkedListShader() : Shader() {
    add_shader_stage<VertexShader>("oit_test.vert");
    add_shader_stage<FragmentShader>("oit_test.frag");
    create();
  }
  void set_color(const glm::vec4 color) { set_uniform("color", color); }
};
//==============================================================================
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
//==============================================================================
class OITTestWindow : public OITWindow {
 public:
  OITTestWindow(const std::string& name)
      : OITWindow(name, screen_width, screen_height),
        cam(30, (float)screen_width / (float)screen_height, 0.01, 100,
            screen_width, screen_height),
        quad1{vec3{-1, -1, 0}, vec3{1, -1, 2}, vec3{-1, 1, 0}, vec3{1, 1, 2}},
        quad2{vec3{-1, -1.4, 1}, vec3{1, -1.4, 1}, vec3{-1, 0.9, 1},
              vec3{1, 0.9, 1}} {
    cam.transform().translate(0, 0, 3);
    set_render_function([&]() {
      shader.bind();
      shader.set_uniform("projection", cam.projection_matrix());
      shader.set_uniform("modelview", cam.view_matrix());

      shader.set_color({1, 0, 0, 0.5});
      quad1.draw();

      shader.set_color({0, 1, 0, 0.5});
      quad2.draw();
    });
  }

 private:
  ColorLinkedListShader shader;
  PerspectiveCamera     cam;
  TriangleStrip         quad1;
  TriangleStrip         quad2;
};

int main() {
  OITTestWindow w("OIT Test");
  w.start_rendering();
}
