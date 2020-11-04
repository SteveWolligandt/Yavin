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
  bool m_closed;
  //==============================================================================
 public:
  display();
  //------------------------------------------------------------------------------
  ~display();
  //==============================================================================
  auto get() const -> auto const& { return m_x_display; }
  auto get() -> auto& { return m_x_display; }
  //----------------------------------------------------------------------------
  void close();
  //----------------------------------------------------------------------------
  void lock();
  void unlock();
};
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
#include <yavin/x11undefs.h>
#endif
