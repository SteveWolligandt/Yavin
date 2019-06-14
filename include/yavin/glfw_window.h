#ifndef YAVIN_GLFW_WINDOW
#define YAVIN_GLFW_WINDOW

#include <functional>
#include <string>
#include <thread>
#include "dllexport.h"
#include "glfunctions.h"
#include <GLFW/glfw3.h>

//==============================================================================
namespace yavin {
//==============================================================================

class glfw_window {
 public:
  DLL_API glfw_window(const std::string& name, const int width,
                 const unsigned int height, const unsigned int major = 4,
                 const unsigned int minor = 5);
  DLL_API ~glfw_window();

  DLL_API void set_key_callback(std::function<void(int, int, int, int)>);
  DLL_API void set_resize_callback(std::function<void(int, int)>);
  DLL_API void set_cursor_pos_callback(std::function<void(double, double)>);
  DLL_API void set_mouse_button_callback(std::function<void(int, int, int)>);
  DLL_API void set_scroll_callback(std::function<void(double, double)>);

  DLL_API void swap_buffers();

  DLL_API void get_framebuffer_size(int& width, int& height);
  DLL_API void poll_events();

  bool should_close() const { return glfwWindowShouldClose(m_window); }
  void should_close(bool b) { glfwSetWindowShouldClose(m_window, b); }

  DLL_API std::string get_gl_version();
  DLL_API std::string get_glsl_version();
  DLL_API void        print_versions();

  DLL_API void make_current();

  auto glfw_window_ptr() const { return m_window; }

 private:
  GLFWwindow*  m_window;

  std::function<void(int, int, int, int)> m_key_callback_function;
  std::function<void(int, int)>           m_resize_callback_function;
  std::function<void(double, double)>     m_cursor_pos_callback_function;
  std::function<void(int, int, int)>      m_mouse_button_callback_function;
  std::function<void(int, int)>           m_scroll_callback_function;
};

//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
