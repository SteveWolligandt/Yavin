#include <yavin/X11/window.h>
//==============================================================================
namespace yavin::x11 {
//==============================================================================
window::window(std::string const& title, size_t width, size_t height,
               XVisualInfo* visual)
    : window{title, width, height, std::make_shared<display>(), visual} {}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
window::window(std::string const& title, size_t width, size_t height,
               std::shared_ptr<display> const& disp, XVisualInfo* visual)
    : m_x_display{disp},
      m_root{DefaultRootWindow(m_x_display->get())},
      m_wm_protocols{XInternAtom(m_x_display->get(), "WM_PROTOCOLS", false)},
      m_wm_delete_window{
          XInternAtom(m_x_display->get(), "WM_DELETE_WINDOW", false)} {
  m_attributes.event_mask =
      ExposureMask | KeyPressMask | ButtonPress | StructureNotifyMask |
      ButtonReleaseMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
      PointerMotionMask | Button1MotionMask | VisibilityChangeMask;
  m_attributes.colormap =
      XCreateColormap(m_x_display->get(), m_root, visual->visual, AllocNone);
  m_x_window = XCreateWindow(m_x_display->get(), m_root, 0, 0, width, height,
                             0,                         // border width
                             visual->depth,             // depth
                             InputOutput,               // class
                             visual->visual,            // visual
                             CWEventMask | CWColormap,  // attribute mask
                             &m_attributes              // attributes
  );
  XStoreName(m_x_display->get(), m_x_window, title.c_str());
  XMapWindow(m_x_display->get(), m_x_window);
  XSetWMProtocols(m_x_display->get(), m_x_window, &m_wm_delete_window, 1);
}
//------------------------------------------------------------------------------
window::~window() {
  XDestroyWindow(m_x_display->get(), m_x_window);
}
//==============================================================================
void window::check_events() {

  while (XCheckWindowEvent(m_x_display->get(), m_x_window,
                           ExposureMask | KeyPressMask | ButtonPress |
                               StructureNotifyMask | ButtonReleaseMask |
                               KeyReleaseMask | EnterWindowMask |
                               LeaveWindowMask | PointerMotionMask |
                               Button1MotionMask | VisibilityChangeMask,
                           &m_xevent)) {
    const auto k = x11_keysym_to_key(
        XkbKeycodeToKeysym(m_x_display->get(), m_xevent.xkey.keycode, 0, 0));
    const auto b = x11_button_to_button(m_xevent.xbutton.button);
    switch (m_xevent.type) {
      case KeyPress: notify_key_pressed(k); break;
      case KeyRelease: notify_key_released(k); break;
      case ButtonPress:
        if (b == BUTTON_WHEEL_UP) {
          notify_wheel_up();
        } else if (b == BUTTON_WHEEL_DOWN) {
          notify_wheel_down();
        } else if (b == BUTTON_WHEEL_LEFT) {
          notify_wheel_left();
        } else if (b == BUTTON_WHEEL_RIGHT) {
          notify_wheel_right();
        } else {
          notify_button_pressed(b);
        }
        break;
      case ButtonRelease: notify_button_released(b); break;
      case MotionNotify:
        notify_mouse_motion(m_xevent.xmotion.x, m_xevent.xmotion.y);
        break;
      case ConfigureNotify:
        notify_resize(m_xevent.xconfigure.width, m_xevent.xconfigure.height);
        break;
    }
    if (XCheckTypedWindowEvent(m_x_display->get(), m_x_window,
                               ClientMessage, &m_xevent)) {
      if (m_xevent.xclient.message_type == m_wm_protocols &&
          static_cast<Atom>(m_xevent.xclient.data.l[0]) == m_wm_delete_window) {
        m_should_close = true;
      }
    }
  }
}
//==============================================================================
}  // namespace yavin::x11
//==============================================================================
