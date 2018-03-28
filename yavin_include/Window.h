#ifndef __YAVIN_WINDOW__
#define __YAVIN_WINDOW__

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include "gl_includes.h"
#include "dll_export.h"

#include <GLFW/glfw3.h>

using namespace std::chrono_literals;

namespace Yavin {
class Window {
 public:
  DLL_API Window();
  DLL_API Window(const std::string& name, const int width, const unsigned int height,
         const unsigned int major = 4, const unsigned int minor = 5);
  DLL_API Window(std::function<void()> render_function);
  DLL_API Window(std::function<void()> render_function, std::function<void(double)> update_function);
  DLL_API ~Window();

  DLL_API void init(const std::string& name, const int width, const unsigned int height,
            const unsigned int major = 4, const unsigned int minor = 5);

  DLL_API void start_rendering();

  DLL_API void set_render_function(std::function<void()>);
  DLL_API void set_update_function(std::function<void(double)>);
  DLL_API void set_key_callback(std::function<void(int, int, int, int)>);
  DLL_API void set_resize_callback(std::function<void(int, int)>);
  DLL_API void set_cursor_pos_callback(std::function<void(double, double)>);
  DLL_API void set_mouse_button_callback(std::function<void(int, int, int)>);

  DLL_API void swap_buffers();

  DLL_API void get_framebuffer_size(int& width, int& height);
  DLL_API void poll_events();

  DLL_API void set_fps(const double fps);
  DLL_API void join_render_thread();
  DLL_API void should_close(bool b);

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