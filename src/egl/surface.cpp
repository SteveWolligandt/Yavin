#include <yavin/egl/surface.h>
//==============================================================================
namespace yavin::egl {
//==============================================================================
surface::surface(std::shared_ptr<egl::display> const &egl_disp, EGLConfig cfg)
    : m_egl_disp{egl_disp} {
  EGLint const pbuffer_attrs[] = {
      EGL_WIDTH,          1,
      EGL_HEIGHT,         1,
      EGL_TEXTURE_TARGET, EGL_NO_TEXTURE,
      EGL_TEXTURE_FORMAT, EGL_NO_TEXTURE,
      EGL_NONE,
  };
  m_egl_surface =
      eglCreatePbufferSurface(m_egl_disp->get(), cfg, pbuffer_attrs);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
surface::surface(std::string const &name, size_t width, size_t height,
                 std::shared_ptr<egl::display> const &egl_disp,
                 std::shared_ptr<x11::display> const &x11_disp, EGLConfig cfg,
                 XVisualInfo *visual_info)
    : m_egl_disp{egl_disp} {
  m_x11_window =
      std::make_shared<x11::window>(name, width, height, x11_disp, visual_info);
  m_egl_surface = eglCreateWindowSurface(m_egl_disp->get(), cfg,
                                         m_x11_window->get(), nullptr);
}
//------------------------------------------------------------------------------
surface::~surface() {
  eglDestroySurface(m_egl_disp->get(), m_egl_surface);
}
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
