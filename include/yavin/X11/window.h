#ifndef YAVIN_X11_WINDOW_H
#define YAVIN_X11_WINDOW_H
//==============================================================================
#include <yavin/X11/display.h>
#include <yavin/window_notifier.h>
#include <yavin/x11buttons.h>
#include <yavin/x11keys.h>
//==============================================================================
namespace yavin::x11 {
//==============================================================================
class window : public window_notifier {
  std::shared_ptr<display> m_x_display;
  XSetWindowAttributes     m_attributes;
  Window                   m_root;
  Window                   m_x_window;
  XEvent                   m_xevent;
  //bool                     m_shift_down, m_ctrl_down, m_alt_down;
  //============================================================================
 public:
  window(std::string const& title, size_t width, size_t height,
         XVisualInfo* visual);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  window(std::string const& title, size_t width, size_t height,
         std::shared_ptr<display> const& disp, XVisualInfo* visual);
  //------------------------------------------------------------------------------
  ~window();
  //==============================================================================
  auto get() const -> auto const& { return m_x_window; }
  auto get() -> auto& { return m_x_window; }
  //------------------------------------------------------------------------------
  void check_events();
};
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
#endif
