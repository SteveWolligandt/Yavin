#ifndef YAVIN_EGL_CONTEXT_H
#define YAVIN_EGL_CONTEXT_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/glincludes.h>
#include <yavin/glew.h>
#include <array>
#include <iostream>
#include <list>
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
class window;
class context {
  public:
    friend class window;
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
  EGLDisplay m_egl_display;
  EGLContext m_egl_context;
  std::unique_ptr<glew> m_glew;

  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  context(EGLint major = 4, EGLint minor = 5);
  context(context&&) noexcept = default;
  auto operator=(context&&) noexcept -> context& = default;
  ~context();

  context(EGLint major, EGLint minor, context const& parent);
  context(EGLint major, EGLint minor, window const& parent);

  //============================================================================
  // methods
  //============================================================================
 public:
  context create_shared_context(int major = 3, int minor = 3) const;
  void           make_current();
  void           release();

 private:
  void setup(EGLint major, EGLint minor, EGLContext parent, bool cur);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
