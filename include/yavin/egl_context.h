#ifndef YAVIN_EGL_CONTEXT_H
#define YAVIN_EGL_CONTEXT_H
//==============================================================================
#include <yavin/egl/context.h>
#include <yavin/egl/display.h>
#include <yavin/egl/surface.h>
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
  // members
  //============================================================================
  std::shared_ptr<egl::environment>             m_egl_env;
  std::shared_ptr<egl::context>                 m_egl_context;
  std::shared_ptr<glew>                         m_glew;

  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  context();
  context(context&&) noexcept = default;
  auto operator=(context&&) noexcept -> context& = default;
  ~context()                                     = default;
  context(context const& parent);
  context(window const& parent);

  //============================================================================
  // methods
  //============================================================================
 public:
  context create_shared_context() const;
  void    make_current();
  void    release();

 private:
  void setup(EGLContext parent, bool cur);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
