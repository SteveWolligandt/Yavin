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
//------------------------------------------------------------------------------
auto display::screen_resolution() const -> std::pair<size_t, size_t> {
  auto screen = XDefaultScreenOfDisplay(m_x_display);
  return {static_cast<size_t>(XWidthOfScreen(screen)),
          static_cast<size_t>(XHeightOfScreen(screen))};
}
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
