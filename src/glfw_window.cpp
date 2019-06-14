#include <yavin/glfw_window.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

//==============================================================================
namespace yavin {
//==============================================================================

glfw_window::glfw_window(const std::string& name, const int width,
               const unsigned int height, const unsigned int major,
               const unsigned int minor)
    : m_key_callback_function([](int, int, int, int) {}),
      m_resize_callback_function([](int, int) {}),
      m_cursor_pos_callback_function([](double, double) {}),
      m_mouse_button_callback_function([](int, int, int) {}),
      m_scroll_callback_function([](double, double) {}) {
  if (!glfwInit()) {
    std::cerr << "glfwInit() failed.\n";
    return;
  } else
    std::cerr << "glfw successfully initialized!.\n";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_SAMPLES, 8);
  m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
  make_current();
  if (!m_window) {
    glfwTerminate();
    throw std::runtime_error{"cannot create GLFW window"};
  }

  glewExperimental = true;
  GLenum err       = glewInit();
  if (GLEW_OK != err) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    throw std::runtime_error{
        std::string{"cannot initialize GLEW: "} +
        std::string{reinterpret_cast<const char*>(glewGetErrorString(err))}};
  }

  glfwSetWindowUserPointer(m_window, this);
  glfwSetKeyCallback(
      m_window, [](GLFWwindow* w, int key, int scancode, int action, int mode) {
        ((glfw_window*)glfwGetWindowUserPointer(w))
            ->m_key_callback_function(key, scancode, action, mode);
      });
  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
    ((glfw_window*)glfwGetWindowUserPointer(w))
        ->m_resize_callback_function(width, height);
  });
  glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
    ((glfw_window*)glfwGetWindowUserPointer(w))
        ->m_cursor_pos_callback_function(x, y);
  });
  glfwSetMouseButtonCallback(
      m_window, [](GLFWwindow* w, int button, int action, int mods) {
        ((glfw_window*)glfwGetWindowUserPointer(w))
            ->m_mouse_button_callback_function(button, action, mods);
      });
  glfwSetScrollCallback(m_window,
                        [](GLFWwindow* w, double xoffset, double yoffset) {
                          ((glfw_window*)glfwGetWindowUserPointer(w))
                              ->m_scroll_callback_function(xoffset, yoffset);
                        });
  print_versions();
}

glfw_window::~glfw_window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void glfw_window::set_key_callback(
    std::function<void(int, int, int, int)> key_callback_function) {
  m_key_callback_function = key_callback_function;
}
void glfw_window::set_resize_callback(
    std::function<void(int, int)> resize_callback_function) {
  m_resize_callback_function = resize_callback_function;
}
void glfw_window::set_cursor_pos_callback(
    std::function<void(double, double)> cursor_pos_callback_function) {
  m_cursor_pos_callback_function = cursor_pos_callback_function;
}
void glfw_window::set_mouse_button_callback(
    std::function<void(int, int, int)> mouse_button_callback_function) {
  m_mouse_button_callback_function = mouse_button_callback_function;
}
void glfw_window::set_scroll_callback(
    std::function<void(double, double)> scroll_callback_function) {
  m_scroll_callback_function = scroll_callback_function;
}

void glfw_window::swap_buffers() { glfwSwapBuffers(m_window); }
void glfw_window::get_framebuffer_size(int& width, int& height) {
  glfwGetFramebufferSize(m_window, &width, &height);
}

void glfw_window::poll_events() { glfwPollEvents(); }

std::string glfw_window::get_gl_version() {
  return reinterpret_cast<const char*>(gl::get_string(GL_VERSION));
}

std::string glfw_window::get_glsl_version() {
  return reinterpret_cast<const char*>(
      gl::get_string(GL_SHADING_LANGUAGE_VERSION));
}
void glfw_window::print_versions() {
  std::cerr << "OpenGL " << get_gl_version() << ", GLSL " << get_glsl_version()
            << "\n";
}
void glfw_window::make_current() { glfwMakeContextCurrent(m_window); }

//==============================================================================
}  // namespace yavin
//==============================================================================
