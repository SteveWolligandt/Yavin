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
  EGLint                        m_egl_major = 4, m_egl_minor = 5;
  //============================================================================
 public:
  environment() : environment{std::make_shared<egl::display>()} {}
  environment(std::shared_ptr<egl::display> const& disp) : m_display{disp} {
    if (!eglInitialize(m_display->get(), &m_egl_major, &m_egl_minor)) {
      throw std::runtime_error{"[EGL] cannot initialize: " +
                               error_string(eglGetError())};
    }
    m_display->bar();
  }
  //============================================================================
  auto display() const -> auto const& { return m_display; }
  auto display() -> auto& { return m_display; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
