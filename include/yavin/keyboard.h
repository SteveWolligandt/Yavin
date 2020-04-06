#ifndef YAVIN_KEYBOARD_H
#define YAVIN_KEYBOARD_H
//==============================================================================
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
enum key {
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_F25,
  KEY_F26,
  KEY_F27,
  KEY_F28,
  KEY_F29,
  KEY_F30,
  KEY_F31,
  KEY_F32,
  KEY_F33,
  KEY_F34,
  KEY_F35,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_BACKSPACE,
  KEY_INSERT,
  KEY_HOME,
  KEY_PAGE_UP,
  KEY_PAGE_DOWN,
  KEY_DELETE,
  KEY_END,
  KEY_TAB,
  KEY_ENTER,
  KEY_KP_ENTER,
  KEY_SPACE,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_ESCAPE,
  KEY_ALT_R,
  KEY_ALT_L,
  KEY_SHIFT_R,
  KEY_SHIFT_L,
  KEY_CTRL_R,
  KEY_CTRL_L,
  KEY_UNKNOWN
};

auto to_string(key k) -> std::string;

struct keyboard_listener {
  virtual void on_key_pressed(key /*k*/) {}
  virtual void on_key_released(key /*k*/) {}
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
