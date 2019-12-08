#ifndef YAVIN_EGL_WINDOW_H
#define YAVIN_EGL_WINDOW_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/glincludes.h>
#include <array>
#include <iostream>
#include <list>
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
class egl_window {
  public:
  //============================================================================
  // static fields
  //============================================================================
  static constexpr std::array<EGLint, 13> attribute_list  {
    EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE,       8,
    EGL_GREEN_SIZE,      8,
    EGL_RED_SIZE,        8,
    EGL_DEPTH_SIZE,      8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };

  //============================================================================
  // fields
  //============================================================================
  EGLDisplay m_egl_display;
  EGLSurface m_egl_surface;
  EGLContext m_egl_context;
  //============================================================================
  // ctors / dtor
  //============================================================================
  egl_window(EGLint w, EGLint h, EGLint major = 4, EGLint minor = 5);
  ~egl_window();
  //============================================================================
  // methods
  //============================================================================
  void swap_buffers() {
    eglSwapBuffers(m_egl_display, m_egl_surface);
  }
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
