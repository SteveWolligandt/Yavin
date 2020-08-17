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
  //==============================================================================
  EGLDisplay   m_egl_display;
  EGLConfig    m_config;
  XVisualInfo* m_visual_info;

  //==============================================================================
 public:
  display() : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
    if (m_egl_display == EGL_NO_DISPLAY) {
      throw std::runtime_error{"[EGL] could not get default display."};
    }
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  display(std::shared_ptr<x11::display> const& x11_disp)
      : m_egl_display{
#ifdef USE_EGL_KHR_PLATFORM_X11
            eglGetPlatformDisplay(EGL_PLATFORM_X11_KHR, x11_disp->get(),
                                  nullptr)
#else
            eglGetDisplay(x11_disp->get())
#endif
        } {
    if (m_egl_display == EGL_NO_DISPLAY) {
      throw std::runtime_error{"[EGL] could not get a display"};
    }
  }
  //------------------------------------------------------------------------------
  ~display() {
    eglTerminate(m_egl_display);
    XFree(m_visual_info);
  }
  //==============================================================================
  auto get() const -> auto const& { return m_egl_display; }
  auto get() -> auto& { return m_egl_display; }
  auto config() const -> auto const& { return m_config; }
  auto visual_info() const -> auto const& { return m_visual_info; }

  void create_config(EGLint* attr_list) {
    EGLint num_cfgs;
    if (!eglChooseConfig(m_egl_display, attr_list, &m_config, 1, &num_cfgs)) {
      throw std::runtime_error{"[EGL] failed to choose config"};
    }
  }
  //------------------------------------------------------------------------------
  void create_config(std::shared_ptr<x11::display> const& x11_disp) {
    XVisualInfo visual_template;
    visual_template.visualid = 0;
    int num_visuals_returned;
    if (!eglGetConfigAttrib(m_egl_display, m_config, EGL_NATIVE_VISUAL_ID,
                            (EGLint*)&visual_template.visualid)) {
      std::cerr << "[EGL] specified attribute list for config has no visual "
                   "id. Trying to found any other.\n";
      EGLint num_configs = 0;
      if (!eglGetConfigs(m_egl_display, NULL, 0, &num_configs)) {
        throw std::runtime_error{"[EGL] could not number of config lists."};
      }
      if (num_configs == 0) {
        throw std::runtime_error{
            "[EGL] no configs in general have been found."};
      }
      std::cerr << "number of configs found " << num_configs << "\n";
      // now we create a buffer to store all our configs
      std::vector<EGLConfig> egl_configs(num_configs);
      // and copy them into our buffer (don't forget to delete once done)
      if (!eglGetConfigs(m_egl_display, egl_configs.data(), num_configs,
                         &num_configs)) {
        throw std::runtime_error{"[EGL] could not get list of all configs."};
      }
      bool any = false;
      for (auto& egl_config : egl_configs) {
        EGLint buffer_size, red_size, blue_size, green_size, alpha_size,
            depth_size, surface_type;

        eglGetConfigAttrib(m_egl_display, egl_config, EGL_BUFFER_SIZE,
                           &buffer_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_RED_SIZE, &red_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_BLUE_SIZE,
                           &green_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_GREEN_SIZE,
                           &blue_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_ALPHA_SIZE,
                           &alpha_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_DEPTH_SIZE,
                           &depth_size);
        eglGetConfigAttrib(m_egl_display, egl_config, EGL_SURFACE_TYPE,
                           &surface_type);
        bool has_window_bit = (surface_type & EGL_WINDOW_BIT) > 0;
        if (buffer_size == 32 && has_window_bit && depth_size == 24 &&
            eglGetConfigAttrib(m_egl_display, egl_config, EGL_NATIVE_VISUAL_ID,
                               (EGLint*)&visual_template.visualid)) {
          m_config = egl_config;
          any      = true;
          break;
        }
      }
      if (!any) {
        throw std::runtime_error{
            "[EGL] could not find any framebuffer configuration for creating a "
            "window."};
      }
    }
    m_visual_info = XGetVisualInfo(x11_disp->get(), VisualIDMask,
                                   &visual_template, &num_visuals_returned);
    if (num_visuals_returned != 1) {
      throw std::runtime_error{
          "[X] number of visual infos is not exactly one (" +
          std::to_string(num_visuals_returned) + "). visual_id is " +
          std::to_string(visual_template.visualid)};
    }
  }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
