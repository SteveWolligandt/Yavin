#ifndef __YAVIN_WINDOW__
#define __YAVIN_WINDOW__

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "gl_includes.h"

#include <GLFW/glfw3.h>

using namespace std::chrono_literals;

namespace Yavin {
class Window {
 public:
  Window();
  Window(const std::string& name, const int width, const unsigned int height,
         const unsigned int major = 4, const unsigned int minor = 5);
  Window(std::function<void()> render_function);
  Window(std::function<void()> render_function, std::function<void(double)> update_function);
  ~Window();

  void init(const std::string& name, const int width, const unsigned int height,
            const unsigned int major = 4, const unsigned int minor = 5);

  void start_rendering();

  void set_render_function(std::function<void()>);
  void set_update_function(std::function<void(double)>);
  void set_key_callback(std::function<void(int, int, int, int)>);
  void set_resize_callback(std::function<void(int, int)>);
  void set_cursor_pos_callback(std::function<void(double, double)>);
  void set_mouse_button_callback(std::function<void(int, int, int)>);

  void swap_buffers();

  void get_framebuffer_size(int& width, int& height);
  void poll_events();

  void set_fps(const double fps);
  void join_render_thread();
  void should_close(bool b);

 private:
  GLFWwindow*  m_window;
  std::thread* m_render_thread = nullptr;

  std::function<void()> m_render_function;
  std::function<void(double dt)> m_update_function;
  std::function<void(int, int, int, int)> m_key_callback_function;
  std::function<void(int, int)>       m_resize_callback_function;
  std::function<void(double, double)> m_cursor_pos_callback_function;
  std::function<void(int, int, int)> m_mouse_button_callback_function;
  double m_fps = 60.0;
};

}  // namespace Yavin
#endif