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
  EGLDisplay   m_egl_display;
  EGLint       m_major = -1, m_minor = -1;

  //============================================================================
 public:
  display() : display{EGL_DEFAULT_DISPLAY} {}
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  display(std::shared_ptr<x11::display> const& x11_disp)
      : display{x11_disp->get()} {}
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  display(NativeDisplayType native_display)
      : m_egl_display{eglGetDisplay(native_display)} {
    if (m_egl_display == EGL_NO_DISPLAY) {
      throw std::runtime_error{"[EGL] could not get a display"};
    }
    if (!eglInitialize(m_egl_display, nullptr, nullptr)) {
      throw std::runtime_error{"[EGL] cannot initialize: " +
                               error_string(eglGetError())};
    }
  }
  //----------------------------------------------------------------------------
  ~display() {
    eglTerminate(m_egl_display);
  }
  //============================================================================
  auto get()         const -> auto const& { return m_egl_display; }
  auto get()               -> auto& { return m_egl_display; }
  //----------------------------------------------------------------------------
  auto version()     const { return std::pair{m_major, m_minor}; }
  //----------------------------------------------------------------------------
  auto create_config(EGLint const* attr_list) {
    EGLint    num_cfgs;
    EGLConfig config;
    if (!eglChooseConfig(m_egl_display, attr_list, &config, 1, &num_cfgs)) {
      throw std::runtime_error{"[EGL] failed to choose config"};
    }
    return config;
  }
  auto num_configs() const{
    EGLint num_configs{};
    if (!eglGetConfigs(m_egl_display, nullptr, 0, &num_configs)) {
      throw std::runtime_error{"[EGL] could not number of config lists."};
    }
    return num_configs;
  }
  auto all_configs() const {
    std::vector<EGLConfig> cfgs;
    if (auto num_cfgs = num_configs(); num_cfgs > 0) {
      cfgs.resize(num_cfgs);
      EGLint aux_num_cfgs;
      if (!eglGetConfigs(m_egl_display, cfgs.data(), num_cfgs, &aux_num_cfgs)) {
        throw std::runtime_error{"[EGL] could not get list of all configs."};
      }
    } else {
      throw std::runtime_error{"[EGL] no configs in general have been found."};
    }
    return cfgs;
  }
  //------------------------------------------------------------------------------
  auto create_visual_info(EGLint const*                        attr_list,
                          std::shared_ptr<x11::display> const& x11_disp) {
    auto         config      = create_config(attr_list);
    XVisualInfo* visual_info = nullptr;
    XVisualInfo  visual_template;
    visual_template.visualid = 0;
    int num_visuals_returned;
    if (!eglGetConfigAttrib(m_egl_display, config, EGL_NATIVE_VISUAL_ID,
                            (EGLint*)&visual_template.visualid)) {
      std::cerr << "[EGL] specified attribute list for config has no visual "
                   "id. Trying to found any other.\n";
      auto cfgs = all_configs();
      bool any  = false;
      for (auto& cfg : cfgs) {
        EGLint buffer_size, red_size, blue_size, green_size, alpha_size,
            depth_size, surface_type;

        eglGetConfigAttrib(m_egl_display, cfg, EGL_BUFFER_SIZE, &buffer_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_RED_SIZE, &red_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_BLUE_SIZE, &green_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_GREEN_SIZE, &blue_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_ALPHA_SIZE, &alpha_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_DEPTH_SIZE, &depth_size);
        eglGetConfigAttrib(m_egl_display, cfg, EGL_SURFACE_TYPE, &surface_type);
        bool has_window_bit = (surface_type & EGL_WINDOW_BIT) > 0;
        if (buffer_size == 32 && has_window_bit && depth_size == 24 &&
            eglGetConfigAttrib(m_egl_display, cfg, EGL_NATIVE_VISUAL_ID,
                               (EGLint*)&visual_template.visualid)) {
          config = cfg;
          any    = true;
          break;
        }
      }
      if (!any) {
        throw std::runtime_error{
            "[EGL] could not find any framebuffer configuration for creating a "
            "window."};
      }
    }
    visual_info = XGetVisualInfo(x11_disp->get(), VisualIDMask,
                                 &visual_template, &num_visuals_returned);
    if (num_visuals_returned != 1) {
      throw std::runtime_error{
          "[X] number of visual infos is not exactly one (" +
          std::to_string(num_visuals_returned) + "). visual_id is " +
          std::to_string(visual_template.visualid)};
    }
    return std::pair{visual_info, config};
  }
};
//==============================================================================
}  // namespace yavin::egl
//==============================================================================
#endif
