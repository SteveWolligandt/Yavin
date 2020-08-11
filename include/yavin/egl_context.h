#ifndef YAVIN_EGL_CONTEXT_H
#define YAVIN_EGL_CONTEXT_H
//==============================================================================
#include <EGL/egl.h>
#include <yavin/egl/display.h>
#include <yavin/egl/environment.h>
#include <yavin/egl/context.h>
#include <yavin/glew.h>
#include <yavin/glincludes.h>

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
  static constexpr std::array<EGLint, 13> attribute_list{
    EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE,       8,
    EGL_GREEN_SIZE,      8,
    EGL_RED_SIZE,        8,
    EGL_DEPTH_SIZE,      8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE};

  //============================================================================
  // members
  //============================================================================
  std::shared_ptr<egl::environment> m_egl_env;
  std::shared_ptr<egl::context>     m_egl_context;
  std::shared_ptr<glew>             m_glew;

  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  context();
  context(context&&) noexcept = default;
  auto operator=(context&&) noexcept -> context& = default;
  ~context()                                     = default;

  context(window const& parent);

  //============================================================================
  // methods
  //============================================================================
 public:
  void    make_current();
  void    release();

 private:
  void setup(EGLContext parent, bool cur);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
