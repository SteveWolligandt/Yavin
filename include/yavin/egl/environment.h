#ifndef YAVIN_EGL_ENVIRONMENT_H
#define YAVIN_EGL_ENVIRONMENT_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/egl/display.h>
#include <yavin/egl/error_string.h>
//==============================================================================
namespace yavin::egl {
//==============================================================================
class environment {
  std::shared_ptr<egl::display> m_display;
  //============================================================================
 public:
  environment(EGLint* attr_list) : m_display{std::make_shared<egl::display>()} {
    if (!eglInitialize(m_display->get(), nullptr, nullptr)) {
      throw std::runtime_error{"[EGL] cannot initialize: " +
                               error_string(eglGetError())};
    }
    m_display->create_config(attr_list);
  }
  environment(std::shared_ptr<x11::display> const& x11_disp)
      : m_display{std::make_shared<egl::display>(x11_disp)} {
    if (!eglInitialize(m_display->get(), nullptr, nullptr)) {
      throw std::runtime_error{"[EGL] cannot initialize: " +
                               error_string(eglGetError())};
    }
    m_display->create_config(x11_disp);
  }
  //============================================================================
  auto display() const -> auto const& { return m_display; }
  auto display() -> auto& { return m_display; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
