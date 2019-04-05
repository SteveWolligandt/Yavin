#include <yavin/window.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

//==============================================================================
namespace yavin {
//==============================================================================

Window::Window(const std::string& name, const int width,
               const unsigned int height, const unsigned int major,
               const unsigned int minor)
    : m_render_function([]() {}),
      m_update_function([](double) {}),
      m_key_callback_function([](int, int, int, int) {}),
      m_resize_callback_function([](int, int) {}),
      m_cursor_pos_callback_function([](double, double) {}),
      m_mouse_button_callback_function([](int, int, int) {}),
      m_scroll_callback_function([](double, double) {}) {
  if (!glfwInit()) {
    std::cerr << "glfwInit() failed.\n";
    return;
  } else
    std::cout << "glfw successfully initialized!.\n";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  glfwWindowHint(GLFW_SAMPLES, 8);
  m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
  make_current();
  if (!m_window) {
    glfwTerminate();
    throw std::runtime_error("cannot create GLFW window");
  }

  glewExperimental = true;
  GLenum err       = glewInit();
  if (GLEW_OK != err) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
    throw std::runtime_error(std::string("cannot initialize GLEW: ") +
                             std::string((char*)glewGetErrorString(err)));
  }

  glfwSetWindowUserPointer(m_window, this);
  glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode,
                                  int action, int mode) {
    ((Window*)glfwGetWindowUserPointer(window))
        ->m_key_callback_function(key, scancode, action, mode);
  });
  glfwSetWindowSizeCallback(m_window,
                            [](GLFWwindow* window, int width, int height) {
                              ((Window*)glfwGetWindowUserPointer(window))
                                  ->m_resize_callback_function(width, height);
                            });
  glfwSetCursorPosCallback(m_window,
                           [](GLFWwindow* window, double x, double y) {
                             ((Window*)glfwGetWindowUserPointer(window))
                                 ->m_cursor_pos_callback_function(x, y);
                           });
  glfwSetMouseButtonCallback(
      m_window, [](GLFWwindow* window, int button, int action, int mods) {
        ((Window*)glfwGetWindowUserPointer(window))
            ->m_mouse_button_callback_function(button, action, mods);
      });
  glfwSetScrollCallback(m_window,
                        [](GLFWwindow* window, double xoffset, double yoffset) {
                          ((Window*)glfwGetWindowUserPointer(window))
                              ->m_scroll_callback_function(xoffset, yoffset);
                        });
  print_versions();
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::set_render_function(std::function<void()> render_function) {
  m_render_function = render_function;
}
void Window::set_update_function(std::function<void(double)> update_function) {
  m_update_function = update_function;
}
void Window::set_key_callback(
    std::function<void(int, int, int, int)> key_callback_function) {
  m_key_callback_function = key_callback_function;
}
void Window::set_resize_callback(
    std::function<void(int, int)> resize_callback_function) {
  m_resize_callback_function = resize_callback_function;
}
void Window::set_cursor_pos_callback(
    std::function<void(double, double)> cursor_pos_callback_function) {
  m_cursor_pos_callback_function = cursor_pos_callback_function;
}
void Window::set_mouse_button_callback(
    std::function<void(int, int, int)> mouse_button_callback_function) {
  m_mouse_button_callback_function = mouse_button_callback_function;
}
void Window::set_scroll_callback(
    std::function<void(double, double)> scroll_callback_function) {
  m_scroll_callback_function = scroll_callback_function;
}

void Window::start_rendering() {
  // m_render_thread = new std::thread([&]() {
  std::cout << "start\n";
  std::chrono::high_resolution_clock timer;
  double                             t = 0.0;
  while (!glfwWindowShouldClose(m_window)) {
    auto time_before = timer.now();

    m_update_function(t);
    m_render_function();
    swap_buffers();
    poll_events();

    auto time_after      = timer.now();
    auto render_duration = time_after - time_before;
    auto render_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(render_duration);

    auto to_wait = 1.0 / (m_fps / 1000.0 - render_time.count());
    if (to_wait > 0)
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(to_wait)));

    t = double(std::chrono::duration_cast<std::chrono::milliseconds>(
                   timer.now() - time_before)
                   .count());
  }
  // });
}

void Window::swap_buffers() { glfwSwapBuffers(m_window); }
void Window::get_framebuffer_size(int& width, int& height) {
  glfwGetFramebufferSize(m_window, &width, &height);
}

void Window::poll_events() { glfwPollEvents(); }

void Window::set_fps(const double fps) { m_fps = fps; }

void Window::join_render_thread() {
  if (m_render_thread) {
    m_render_thread->join();
    delete m_render_thread;
    m_render_thread = nullptr;
  }
}

std::string Window::get_gl_version() {
  return (char*)gl::get_string(GL_VERSION);
}
std::string Window::get_glsl_version() {
  return (char*)gl::get_string(GL_SHADING_LANGUAGE_VERSION);
}
void Window::print_versions() {
  std::cout << "OpenGL " << get_gl_version() << ", GLSL " << get_glsl_version()
            << "\n";
}
void Window::should_close(bool b) { glfwSetWindowShouldClose(m_window, b); }
void Window::make_current() { glfwMakeContextCurrent(m_window); }

//==============================================================================
}  // namespace yavin
//==============================================================================
