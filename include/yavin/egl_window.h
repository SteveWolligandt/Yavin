#ifndef YAVIN_EGL_CONTEXT_H
#define YAVIN_EGL_CONTEXT_H
//==============================================================================
#include <yavin/glincludes.h>

#include <EGL/egl.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <array>
#include <iostream>
#include <list>
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
class window {
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
  // members
  //============================================================================
  Display *  x_display;
  Window     win;
  EGLDisplay m_egl_display;
  EGLContext m_egl_context;
  bool m_glew_initialized = false;

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
  //context create_shared_context(EGLint major = 4, EGLint minor = 5) const;
  void           make_current();
  void           release();

 private:
  void setup(const std::string &title, GLsizei width, GLsizei height,
             EGLint major, EGLint minor, bool cur);
  void setup_x(const std::string &title, GLsizei width, GLsizei height);
  void setup_egl(EGLint major, EGLint minor, bool cur);
  void init_glew();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
