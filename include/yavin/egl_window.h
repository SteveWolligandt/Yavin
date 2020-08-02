#ifndef YAVIN_EGL_WINDOW_H
#define YAVIN_EGL_WINDOW_H
//==============================================================================
#include <yavin/glincludes.h>
#include <yavin/glew.h>

#include <memory>
#include <EGL/egl.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <array>
#include <iostream>
#include <list>
#include <string>

#include <yavin/imgui.h>
#include <yavin/egl_context.h>
#include "window_listener.h"
#include "window_notifier.h"
//==============================================================================
namespace yavin {
//==============================================================================
std::string egl_error_to_string(EGLint error);
//==============================================================================
class context;
class window : public window_notifier {
 public:
   friend class context;
  //============================================================================
  // static fields
  //============================================================================
  static constexpr std::array<EGLint, 3> attribute_list  {
    EGL_BUFFER_SIZE,        32,
    //EGL_RED_SIZE,            8,
    //EGL_GREEN_SIZE,          8,
    //EGL_BLUE_SIZE,           8,
    //EGL_ALPHA_SIZE,          8,
    //
    //EGL_DEPTH_SIZE,         EGL_DONT_CARE,
    //EGL_STENCIL_SIZE,       EGL_DONT_CARE,
    //
    //EGL_RENDERABLE_TYPE,    EGL_OPENGL_BIT,
    //EGL_SURFACE_TYPE,       EGL_WINDOW_BIT | EGL_PIXMAP_BIT,
    EGL_NONE,
  };

  //============================================================================
  // members
  //============================================================================
  Display *                             m_x_display;
  Window                                m_x_window;
  EGLDisplay                            m_egl_display;
  EGLContext                            m_egl_context;
  EGLSurface                            m_egl_surface;
  std::unique_ptr<glew> m_glew;
  std::unique_ptr<imgui_render_backend> m_imgui_render_backend;
  bool                                  m_shift_down, m_ctrl_down, m_alt_down;
  XEvent                                m_xevent;

  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  window(const std::string &title, GLsizei width, GLsizei height,
             EGLint major = 4, EGLint minor = 5);
  ~window();

  //============================================================================
  // methods
  //============================================================================
 public:
  context create_shared_context(int major = 3, int minor = 3) const;
  void make_current();
  void release();
  void refresh();
  void render_imgui();
  void check_events();
  void swap_buffers();

 private:
  void setup(const std::string &title, GLsizei width, GLsizei height,
             EGLint major, EGLint minor);
  void init_imgui(size_t width, size_t height);
  void deinit_imgui();
};
  //==============================================================================
}  // namespace yavin
//==============================================================================
#endif
