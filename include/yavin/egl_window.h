#ifndef YAVIN_EGL_WINDOW_H
#define YAVIN_EGL_WINDOW_H
//==============================================================================
#include <yavin/egl/context.h>
#include <yavin/egl/display.h>
#include <yavin/egl/surface.h>
#include <yavin/egl_context.h>
#include <yavin/glew.h>
#include <yavin/glincludes.h>
#include <yavin/imgui.h>

#include <array>
#include <iostream>
#include <list>
#include <memory>
#include <string>

#include "window_listener.h"
#include "window_notifier.h"
//==============================================================================
namespace yavin {
//==============================================================================
class context;
class window : public window_notifier, public window_listener {
 public:
   friend class context;
  //============================================================================
  // static fields
  //============================================================================

  //============================================================================
  // members
  //============================================================================
  std::shared_ptr<egl::environment>     m_egl_env;
  std::shared_ptr<egl::context>         m_egl_context;
  std::shared_ptr<egl::surface>         m_egl_surface;
  std::shared_ptr<glew>                 m_glew;
  std::unique_ptr<imgui_render_backend> m_imgui_render_backend;
  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  window(const std::string &title, GLsizei width, GLsizei height);
  ~window();

  //============================================================================
  // methods
  //============================================================================
 public:
  context create_shared_context() const;
  void    make_current();
  void    release();
  void    refresh();
  void    render_imgui();
  void    swap_buffers();
  void    check_events();
  void    on_key_pressed(key /*k*/) override;
  void    on_key_released(key /*k*/) override;
  void    on_button_pressed(button /*b*/) override;
  void    on_button_released(button /*b*/) override;
  void    on_mouse_motion(int /*x*/, int /*y*/) override;
  void    on_resize(int /*width*/, int /*height*/) override;

 private:
  void setup(const std::string &title, GLsizei width, GLsizei height);
  void init_imgui(size_t width, size_t height);
  void deinit_imgui();
};
  //==============================================================================
}  // namespace yavin
//==============================================================================
#endif
