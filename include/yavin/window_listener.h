#ifndef YAVIN_WINDOW_LISTENER_H
#define YAVIN_WINDOW_LISTENER_H
//==============================================================================
#include "keyboard.h"
#include "mouse.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct window_listener : keyboard_listener, button_listener {
  virtual void on_mouse_motion(int /*x*/, int /*y*/) {}
  virtual void on_resize(int /*width*/, int /*height*/) {}
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
