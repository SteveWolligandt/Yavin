#include <iostream>
#include <yavin>
//==============================================================================
struct listener : yavin::window_listener {
  bool&         m_run;
  GLsizei &m_width, m_height;
  //----------------------------------------------------------------------------
  listener(bool& run, GLsizei& width, GLsizei& height)
      : m_run{run}, m_width{width}, m_height{height} {}
  //----------------------------------------------------------------------------
  void on_key_pressed(yavin::key k) override {
    std::cerr << "key pressed " << to_string(k) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_key_released(yavin::key k) override {
    if (k == yavin::KEY_ESCAPE) { m_run = false; }
  }
  //----------------------------------------------------------------------------
  void on_button_pressed(yavin::button b) override {
    std::cerr << "button pressed " << to_string(b) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_button_released(yavin::button b) override {
    std::cerr << "button released " << to_string(b) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_mouse_motion(int x, int y) override {
    std::cerr << "mouse moved " << x << ", " << y << "\n";
  }
  //----------------------------------------------------------------------------
  void on_resize(int w, int h) override {
    std::cerr << "resized " << w << ", " << h << "\n";
    m_width  = w;
    m_height = h;
  }
};
//==============================================================================
void imgui () {
  static bool  my_tool_active = true;
  static float f              = 0.0f;
  static float col[3];
  static char  str0[128] = "Hello, world!";
  ImGui::Begin("foo", &my_tool_active);
  ImGui::Text("Hello, world %d", 123);
  ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
  ImGui::SliderFloat("float", &f, 0.0f, 255.0f);
  ImGui::ColorEdit3("Color", col);
  ImGui::End();
}
//------------------------------------------------------------------------------
int main() {
  bool         run = true;
  GLsizei w = 1000, h = 1000;

  yavin::window window{"glx window test", w, h};
  listener      l{run, w, h};
  window.add_listener(l);
  yavin::gl::clear_color(255, 255, 255, 255);
  while (run) {
    yavin::clear_color_buffer();
    window.refresh();
    yavin::gl::viewport(0, 0, w, h);

    imgui();
    window.render_imgui();
    
    window.swap_buffers();
  }
}
