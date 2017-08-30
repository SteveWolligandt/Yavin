#include <iostream>
#include <memory>
#include <thread>
#include <yavin>

using namespace std::chrono_literals;

static unsigned int window_width  = 400;
static unsigned int window_height = 400;

class TestMesh : public Yavin::Mesh<Yavin::vec3, Yavin::vec3, Yavin::vec2>, public Yavin::Movable {
 public:
  TestMesh();
  void draw();
  void update(double dt);
  void set_brightness(float brightness);

  Yavin::Texture2D<uint8_t, Yavin::RGB> m_tex1;
  Yavin::Texture2D<uint8_t, Yavin::RGB> m_tex2;
  Yavin::Texture2D<uint8_t, Yavin::RGB> m_fbo_tex;
  Yavin::FrameBuffer m_fbo;
  Yavin::Shader      m_shader;

  float m_counter = 0.0;

  static constexpr unsigned int fbo_width  = 1600;
  static constexpr unsigned int fbo_height = 1200;
};

TestMesh::TestMesh()
    : Yavin::Mesh<Yavin::vec3, Yavin::vec3, Yavin::vec2>("bunny.obj"),
      m_tex1("rgb.png"),
      m_tex2("rgb2.png"),
      m_fbo_tex(1600, 1200) {
  // setting texture properties
  m_tex1.set_interpolation_mode(Yavin::Texture::LINEAR);
  m_tex1.set_wrap_mode(Yavin::Texture::REPEAT);

  m_tex2.set_interpolation_mode(Yavin::Texture::LINEAR);
  m_tex2.set_wrap_mode(Yavin::Texture::REPEAT);

  m_fbo_tex.set_interpolation_mode(Yavin::Texture::LINEAR);
  m_fbo_tex.set_wrap_mode(Yavin::Texture::REPEAT);

  m_fbo.attachTex2D(m_fbo_tex, 0);

  m_shader.add_shader_stage<Yavin::VertexShader>("shaders/passthrough.vert");
  m_shader.add_shader_stage<Yavin::FragmentShader>("shaders/passthrough.frag");
  m_shader.create();
  m_shader.set_uniform("tex1", 0);
  m_shader.set_uniform("tex2", 1);
}

void TestMesh::draw() {
  m_fbo.bind();
  Yavin::clear_color_buffer();
  Yavin::set_viewport(0, 0, fbo_width, fbo_height);

  m_tex1.bind(0);
  m_tex2.bind(1);
  Yavin::Mesh<Yavin::vec3, Yavin::vec3, Yavin::vec2>::draw();

  Yavin::FrameBuffer::unbind();
  Yavin::clear_color_buffer();
  Yavin::set_viewport(0, 0, window_width, window_height);

  Yavin::Mesh<Yavin::vec3, Yavin::vec3, Yavin::vec2>::draw();
}

void TestMesh::update(double dt) {
  if (dt > 0) m_counter += static_cast<float>(dt) / 1000.0f * 3.14159f;
  set_brightness(sin(m_counter) * 0.5f + 0.5f);
}

void TestMesh::set_brightness(float brightness) { m_shader.set_uniform("brightness", brightness); }

std::thread* png_saver_thread = nullptr;
bool         w_down = false, s_down = false;
bool         a_down = false, d_down = false;
bool         q_down = false, e_down = false;
bool         left_mouse_down = false;
double       mouse_x_old, mouse_y_old;
double       mouse_x, mouse_y;
glm::vec3    eye(0, 0, -10);
glm::vec3    center(0, 0, 0);
glm::vec3    up(0, 1, 0);
int main(int argc, char** argv) {
  Yavin::Window w("Test Window", window_width, window_height, 4, 5);
  Yavin::enable_depth_test();
  TestMesh                 model;
  Yavin::PerspectiveCamera cam(30, (float)window_width / (float)window_height, 0.0001f, 100.0f);
  cam.set_projection(20, (float)window_width / (float)window_height, 0.0001f, 100.0f);
  cam.transform().first_person(glm::vec3(0, 0, -5), 0, 180);

  w.set_render_function([&]() {
    Yavin::clear_color_depth_duffer();
    // model.m_shader.bind();
    model.m_shader.set_uniform("modelview", cam.view_matrix());
    model.m_shader.set_uniform("projection", cam.projection_matrix());
    model.draw();

    w.swap_buffers();
    w.poll_events();
  });

  w.set_key_callback([&](int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) s_down   = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) s_down = false;
    if (key == GLFW_KEY_W && action == GLFW_PRESS) w_down   = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) w_down = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) a_down   = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) a_down = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) d_down   = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) d_down = false;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) q_down   = true;
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) q_down = false;
    if (key == GLFW_KEY_E && action == GLFW_PRESS) e_down   = true;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) e_down = false;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) w.should_close(true);
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
      if (png_saver_thread) {
        png_saver_thread->join();
        delete png_saver_thread;
      }
      model.m_fbo_tex.download_data();
      png_saver_thread = new std::thread([&]() { model.m_fbo_tex.save_png("screenshot.png"); });
    }
  });

  w.set_resize_callback([&](int w, int h) {
    cam.set_projection(20, (float)w / (float)h, 0.0001f, 100.0f);
    window_width  = w;
    window_height = h;
    std::cout << w << ", " << h << '\n';
  });

  w.set_update_function([&](double dt) {
    if (s_down) cam.transform().translate(0, 0, 1 / dt);
    if (w_down) cam.transform().translate(0, 0, -1 / dt);
    if (a_down) cam.transform().translate(-1 / dt, 0, 0);
    if (d_down) cam.transform().translate(1 / dt, 0, 0);
    if (q_down) cam.transform().translate(0, 1 / dt, 0);
    if (e_down) cam.transform().translate(0, -1 / dt, 0);

    // model.m_shader.set_uniform("modelview", glm::lookAt(eye, center, up));
    model.update(dt);
  });

  w.set_mouse_button_callback([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) left_mouse_down   = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) left_mouse_down = false;

    mouse_x_old = mouse_x;
    mouse_y_old = mouse_y;
  });

  w.set_cursor_pos_callback([&](double x, double y) {
    mouse_x = x;
    mouse_y = y;

    if (left_mouse_down) {
      double x_offset = mouse_x - mouse_x_old;
      double y_offset = mouse_y - mouse_y_old;

      mouse_x_old = mouse_x;
      mouse_y_old = mouse_y;

      cam.transform().inc_yaw(-x_offset);
      cam.transform().inc_pitch(-y_offset);
    }
  });

  w.set_fps(120);
  w.start_rendering();
  // w.join_render_thread();
  if (png_saver_thread) {
    png_saver_thread->join();
    delete png_saver_thread;
  }
  return 0;
}