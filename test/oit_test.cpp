#include <yavin>
#include "TriangleStrip.h"

using namespace Yavin;

const unsigned int screen_width  = 800;
const unsigned int screen_height = 600;

//==============================================================================
namespace Yavin::Test {
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

class OITTestWindow : public OITWindow {
 public:
  OITTestWindow(const std::string& name, unsigned int linked_list_size_factor)
      : OITWindow(name, screen_width, screen_height, linked_list_size_factor),
        cam(30, (float)screen_width / (float)screen_height, 0.01, 100,
            screen_width, screen_height),
        quads{
            {{{-1, -1, 0}}, {{1, -1, 2}}, {{-1, 1, 0}}, {{1, 1, 2}}},
            {{{-1.1, -0.9, 1}},
             {{0.9, -0.9, 1}},
             {{-1, 1.4, 1}},
             {{1, 1.4, 1}}},
            {{{-2, -0.9, 1}},
             {{1.1, -0.9, 0}},
             {{-2, 1.1, 1}},
             {{1.1, 1.1, 0}}},
        },
        colors{{1.f, 0.f, 0.f, 0.5f},
               {0.f, 0.f, 1.f, 0.5f},
               {0.f, 1.f, 0.f, 0.5f}} {
    cam.transform().translate(0, 0, 3);
    set_clear_color({1, 1, 1, 1});
    set_render_function([&]() {
      shader.bind();
      shader.set_uniform("projection", cam.projection_matrix());
      shader.set_uniform("modelview", cam.view_matrix());

      for (size_t i = 0; i < quads.size(); ++i) {
        shader.set_color(colors[i]);
        quads[i].draw();
      }
    });

    set_resize_callback([this](int w, int h) {
      cam.set_projection(30, (float)w / (float)h, 0.01, 100, w, h);
    });
  }

 private:
  ColorLinkedListShader      shader;
  PerspectiveCamera          cam;
  std::vector<TriangleStrip> quads;
  std::vector<glm::vec4>     colors;
};
//==============================================================================
}  // namespace Yavin::Test
//==============================================================================

int main() {
  Yavin::Test::OITTestWindow w("OIT Test", 20);
  w.start_rendering();
}
