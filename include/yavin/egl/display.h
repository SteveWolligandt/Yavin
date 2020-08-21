#ifndef YAVIN_EGL_DISPLAY_H
#define YAVIN_EGL_DISPLAY_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/X11/display.h>
#include <yavin/egl/error_string.h>

#include <iostream>
#include <memory>
#include <vector>
//==============================================================================
namespace yavin::egl {
//==============================================================================
class display {
  EGLDisplay m_egl_display;
  EGLint     m_major = -1, m_minor = -1;

  //============================================================================
 public:
  display();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  display(std::shared_ptr<x11::display> const& x11_disp);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  display(NativeDisplayType native_display);
  //----------------------------------------------------------------------------
  ~display();
  //============================================================================
  auto get() const -> auto const& { return m_egl_display; }
  auto get() -> auto& { return m_egl_display; }
  //----------------------------------------------------------------------------
  auto      version() const { return std::pair{m_major, m_minor}; }
  EGLConfig create_config(EGLint const* attr_list);
  EGLint    num_configs() const;
  std::vector<EGLConfig> all_configs() const;
  //------------------------------------------------------------------------------
  std::pair<XVisualInfo*, EGLConfig> create_visual_info(
      EGLint const* attr_list, std::shared_ptr<x11::display> const& x11_disp);
  };
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
