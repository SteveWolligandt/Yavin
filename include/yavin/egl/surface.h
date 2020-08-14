#ifndef YAVIN_EGL_SURFACE_H
#define YAVIN_EGL_SURFACE_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/X11/window.h>
#include <yavin/egl/display.h>
#include <yavin/egl/environment.h>
//==============================================================================
namespace yavin::egl {
//==============================================================================
class surface {
  std::shared_ptr<egl::display> m_egl_display;
  std::shared_ptr<x11::window>  m_x11_window;
  EGLSurface                    m_egl_surface;
  //==============================================================================
 public:
  surface(std::string const &name, size_t width, size_t height,
          std::shared_ptr<egl::display> const &egl_disp,
          std::shared_ptr<x11::display> const &x11_disp) {
    m_egl_display = egl_disp;
    m_x11_window  = std::make_shared<x11::window>(name, width, height, x11_disp,
                                                 m_egl_display->visual_info());
    m_egl_surface =
        eglCreateWindowSurface(m_egl_display->get(), m_egl_display->config(),
                               m_x11_window->get(), nullptr);
  }
  surface(std::string const &name, size_t width, size_t height,
          std::shared_ptr<x11::display> const &x11_disp) {
    m_egl_display = std::make_shared<egl::display>(x11_disp);
    m_x11_window  = std::make_shared<x11::window>(name, width, height, x11_disp,
                                                 m_egl_display->visual_info());
    m_egl_surface =
        eglCreateWindowSurface(m_egl_display->get(), m_egl_display->config(),
                               m_x11_window->get(), nullptr);
  }
  //------------------------------------------------------------------------------
  ~surface() {
    eglDestroySurface(m_egl_display->get(), m_egl_surface);
  }
  //==============================================================================
  auto get() const -> auto const&{ return m_egl_surface; }
  auto get() -> auto &{ return m_egl_surface; }
  auto x11_window() const -> auto const & { return m_x11_window; }
  auto x11_window() -> auto & { return m_x11_window; }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
