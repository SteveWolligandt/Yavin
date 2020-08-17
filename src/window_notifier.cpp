#include <yavin/window_notifier.h>
#include <iostream>
//==============================================================================
namespace yavin {
//==============================================================================
void window_notifier::add_listener(window_listener &l) {
  m_window_listeners.push_back(&l);
}
//------------------------------------------------------------------------------
void window_notifier::notify_key_pressed(key k) {
  for (auto l : m_window_listeners) { l->on_key_pressed(k); }
}
//------------------------------------------------------------------------------
void window_notifier::notify_key_released(key k) {
  for (auto l : m_window_listeners) { l->on_key_released(k); }
}
//------------------------------------------------------------------------------
void window_notifier::notify_button_pressed(button b) {
  for (auto l : m_window_listeners) { l->on_button_pressed(b); }
}
//------------------------------------------------------------------------------
void window_notifier::notify_button_released(button b) {
  for (auto l : m_window_listeners) { l->on_button_released(b); }
}
void window_notifier::notify_wheel_up() {
  for (auto l : m_window_listeners) { l->on_wheel_up(); }
}
void window_notifier::notify_wheel_down() {
  for (auto l : m_window_listeners) { l->on_wheel_down(); }
}
void window_notifier::notify_wheel_left() {
  for (auto l : m_window_listeners) { l->on_wheel_left(); }
}
void window_notifier::notify_wheel_right() {
  for (auto l : m_window_listeners) { l->on_wheel_right(); }
}
//------------------------------------------------------------------------------
void window_notifier::notify_mouse_motion(int x, int y) {
  for (auto l : m_window_listeners) { l->on_mouse_motion(x, y); }
}
//------------------------------------------------------------------------------
void window_notifier::notify_resize(int width, int height) {
  for (auto l : m_window_listeners) { l->on_resize(width, height); }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
