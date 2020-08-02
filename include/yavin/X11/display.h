#ifndef YAVIN_X11_DISPLAY_H
#define YAVIN_X11_DISPLAY_H
//==============================================================================
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdexcept>
//==============================================================================
namespace yavin::x11 {
//==============================================================================
class display {
  Display* m_x_display;
  //==============================================================================
 public:
  display() : m_x_display{XOpenDisplay(nullptr)} {
    if (m_x_display == nullptr) {
      throw std::runtime_error{"[X] cannot connect server"};
    }
  }
  //------------------------------------------------------------------------------
  ~display() { XCloseDisplay(m_x_display); }
  //==============================================================================
  auto get() const -> auto const& { return m_x_display; }
  auto get() -> auto& { return m_x_display; }
};
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
#endif
