#ifndef YAVIN_WINDOW_NOTIFIER_H
#define YAVIN_WINDOW_NOTIFIER_H
//==============================================================================
#include <vector>
#include "window_listener.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct window_notifier {
  std::vector<window_listener *> m_window_listeners;
  //----------------------------------------------------------------------------
  void add_listener(window_listener &l);
  //----------------------------------------------------------------------------
  void notify_key_pressed(key k);
  void notify_key_released(key k);
  void notify_button_pressed(button b);
  void notify_button_released(button b);
  void notify_wheel_up();
  void notify_wheel_down();
  void notify_wheel_left();
  void notify_wheel_right();
  void notify_mouse_motion(int x, int y);
  void notify_resize(int width, int height);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
