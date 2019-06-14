#include <yavin/apis.h>
#include <yavin/glfw_context.h>
#include <iostream>

//==============================================================================
namespace yavin {
//==============================================================================

glfw_context::glfw_context(unsigned int major, unsigned int minor,
                           unsigned int width, unsigned int height) {
  apis::glfw::init();

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  m_window = glfwCreateWindow(width, height, "", nullptr, nullptr);
  make_current();
  if (m_window == nullptr) {
    throw std::runtime_error{"cannot create GLFW window"};
  }

  apis::glew::init();
}

glfw_context::glfw_context(const glfw_window& w, unsigned int major,
                           unsigned int minor, unsigned int width,
                           unsigned int height) {
  apis::glfw::init();

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  m_window = glfwCreateWindow(width, height, "", nullptr, w.glfw_window_ptr());
  make_current();
  if (m_window == nullptr) {
    throw std::runtime_error{"cannot create GLFW window"};
  }

  apis::glew::init();
}

glfw_context::~glfw_context() {
  glfwDestroyWindow(m_window);
}

void glfw_context::make_current() {
  glfwMakeContextCurrent(m_window);
}

//==============================================================================
}  // namespace yavin
//==============================================================================
