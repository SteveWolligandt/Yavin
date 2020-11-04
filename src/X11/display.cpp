#include <yavin/X11/display.h>
//==============================================================================
namespace yavin::x11 {
//==============================================================================
display::display() {
  XInitThreads();
  m_x_display = XOpenDisplay(nullptr);
  m_closed    = false;
  if (m_x_display == nullptr) {
    throw std::runtime_error{"[X] cannot connect server"};
  }
}
//------------------------------------------------------------------------------
void display::close() {
  if (!m_closed) {
    XCloseDisplay(m_x_display);
    m_closed = true;
  }
}
//------------------------------------------------------------------------------
display::~display() {
  close();
}
//------------------------------------------------------------------------------
void display::lock() { XLockDisplay(m_x_display); }
void display::unlock() { XUnlockDisplay(m_x_display); }
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
