#ifndef YAVIN_WINDOW_NOTIFIER_H
#define YAVIN_WINDOW_NOTIFIER_H
//==============================================================================
#include <vector>
#include <yavin/holder.h>
#include "window_listener.h"
//==============================================================================
namespace yavin {
//==============================================================================
template <typename Event>
struct key_pressed_event : holder<Event>, yavin::window_listener {
  using holder<Event>::holder;
  void on_key_pressed(yavin::key k) override {
    this->get()(k);
  }
};
// copy when having rvalue
template <typename T>
key_pressed_event(T &&) -> key_pressed_event<T>;
// keep reference when having lvalue
template <typename T>
key_pressed_event(T const&) -> key_pressed_event<T const&>;
//==============================================================================
template <typename Event>
struct key_released_event : holder<Event>, yavin::window_listener {
  using holder<Event>::holder;
  void on_key_pressed(yavin::key k) override {
    this->get()(k);
  }
};
// copy when having rvalue
template <typename T>
key_released_event(T &&) -> key_released_event<T>;
// keep reference when having lvalue
template <typename T>
key_released_event(T const&) -> key_released_event<T const &>;
//==============================================================================
template <typename Event>
struct button_pressed_event : holder<Event>, yavin::window_listener {
  using holder<Event>::holder;
  void on_button_pressed(yavin::button b) override {
    this->get()(b);
  }
};
// copy when having rvalue
template <typename T>
button_pressed_event(T &&) -> button_pressed_event<T>;
// keep reference when having lvalue
template <typename T>
button_pressed_event(T const &) -> button_pressed_event<T const &>;
//==============================================================================
template <typename Event>
struct button_released_event : holder<Event>, yavin::window_listener {
  using holder<Event>::holder;
  void on_button_released(yavin::button b) override {
    this->get()(b);
  }
};
// copy when having rvalue
template <typename T>
button_released_event(T &&) -> button_released_event<T>;
// keep reference when having lvalue
template <typename T>
button_released_event(T const &) -> button_released_event<T const &>;
//==============================================================================
template <typename Event>
struct mouse_motion_event : holder<Event>, yavin::window_listener {
  using holder<Event>::holder;
  void on_mouse_motion(int x, int y) override {
    this->get()(x, y);
  }
};
// copy when having rvalue
template <typename T>
mouse_motion_event(T &&) -> mouse_motion_event<T>;
// keep reference when having lvalue
template <typename T>
mouse_motion_event(T const &) -> mouse_motion_event<T const &>;
//==============================================================================
struct window_notifier {
  std::vector<window_listener *> m_window_listeners;
  std::vector<std::unique_ptr<base_holder>> m_events;
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
  //----------------------------------------------------------------------------
  void add_listener(window_listener &l);
  //----------------------------------------------------------------------------
  template <typename Event>
  void add_key_pressed_event(Event&& event) {
    m_events.push_back(std::unique_ptr<base_holder>{
        new key_pressed_event{std::forward<Event>(event)}});
    add_listener(*dynamic_cast<yavin::window_listener*>(m_events.back().get()));
  }
  //----------------------------------------------------------------------------
  template <typename Event>
  void add_key_released_event(Event&& event) {
    m_events.push_back(std::unique_ptr<base_holder>{
        new key_released_event{std::forward<Event>(event)}});
    add_listener(*dynamic_cast<yavin::window_listener*>(m_events.back().get()));
  }
  //----------------------------------------------------------------------------
  template <typename Event>
  void add_button_pressed_event(Event&& event) {
    m_events.push_back(std::unique_ptr<base_holder>{
        new button_pressed_event{std::forward<Event>(event)}});
    add_listener(*dynamic_cast<yavin::window_listener*>(m_events.back().get()));
  }
  //----------------------------------------------------------------------------
  template <typename Event>
  void add_button_released_event(Event&& event) {
    m_events.push_back(std::unique_ptr<base_holder>{
        new button_released_event{std::forward<Event>(event)}});
    add_listener(*dynamic_cast<yavin::window_listener*>(m_events.back().get()));
  }
  //----------------------------------------------------------------------------
  template <typename Event>
  void add_mouse_motion_event(Event&& event) {
    m_events.push_back(std::unique_ptr<base_holder>{
        new mouse_motion_event{std::forward<Event>(event)}});
    add_listener(*dynamic_cast<yavin::window_listener*>(m_events.back().get()));
  }
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
