#ifndef YAVIN_WINDOW_H
#define YAVIN_WINDOW_H
//==============================================================================
#include <yavin/egl/context.h>
#include <yavin/egl/display.h>
#include <yavin/egl/surface.h>
#include <yavin/context.h>
#include <yavin/glew.h>
#include <yavin/glincludes.h>
#include <yavin/imgui.h>
#include <yavin/window_listener.h>
#include <yavin/window_notifier.h>
#include <yavin/X11/display.h>

#include <array>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
//==============================================================================
namespace yavin {
//==============================================================================
class context;
class window : public window_notifier, public window_listener {
 public:
  friend class context;
  //============================================================================
  // members
  //============================================================================
  std::shared_ptr<egl::display>                 m_egl_disp;
  std::shared_ptr<egl::context>                 m_egl_context;
  std::shared_ptr<egl::surface>                 m_egl_surface;
  std::shared_ptr<glew>                         m_glew;
  std::unique_ptr<imgui_render_backend>         m_imgui_render_backend;
  std::list<std::thread>                        m_async_tasks;
  std::vector<std::list<std::thread>::iterator> m_joinable_async_tasks;
  std::mutex                                    m_async_tasks_mutex;
  mutable std::mutex                                    m_context_creation_mutex;
  //============================================================================
  // ctors / dtor
  //============================================================================
 public:
  window(const std::string &title, size_t width, size_t height);
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
  void    on_wheel_up() override;
  void    on_wheel_down() override;
  void    on_wheel_left() override;
  void    on_wheel_right() override;
  void    on_mouse_motion(int /*x*/, int /*y*/) override;
  void    on_resize(int /*width*/, int /*height*/) override;
  //----------------------------------------------------------------------------
  auto should_close() const {
    return m_egl_surface->x11_window()->should_close();
  }
  //----------------------------------------------------------------------------
  template <typename F>
  void do_async(F &&f) {
    auto it = [this] {
      std::lock_guard lock{m_async_tasks_mutex};
      m_async_tasks.emplace_back();
      return prev(m_async_tasks.end());
    }();

    m_async_tasks.back() = std::thread{[&w = *this, it, g = f] {
      w.x_init_threads();
      auto ctx = w.create_shared_context();
      ctx.make_current();
      g();
      std::lock_guard lock{w.m_async_tasks_mutex};
      w.m_joinable_async_tasks.push_back(it);
    }};
  }

 private:
  void setup(const std::string &title, size_t width, size_t height);
  void init_imgui(size_t width, size_t height);
  void deinit_imgui();
  void x_init_threads();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
