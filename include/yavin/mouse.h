#ifndef YAVIN_MOUSE_H
#define YAVIN_MOUSE_H
//==============================================================================
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
enum button {
  BUTTON_LEFT, BUTTON_RIGHT, BUTTON_MIDDLE, BUTTON_UNKNOWN
};

auto to_string(button b) -> std::string;

struct button_listener {
  virtual void on_button_pressed(button /*b*/) {}
  virtual void on_button_released(button /*b*/) {}
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
