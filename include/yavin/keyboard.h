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
  KEY_LEFT,
  KEY_RIGHT,
  KEY_UP,
  KEY_DOWN,
  KEY_ESCAPE,
  KEY_UNKNOWN
};

std::string to_string(key k) {
  switch (k) {
    case key::KEY_0: return "0";
    case key::KEY_1: return "1";
    case key::KEY_2: return "2";
    case key::KEY_3: return "3";
    case key::KEY_4: return "4";
    case key::KEY_5: return "5";
    case key::KEY_6: return "6";
    case key::KEY_7: return "7";
    case key::KEY_8: return "8";
    case key::KEY_9: return "9";
    case key::KEY_F1: return "F1";
    case key::KEY_F2: return "F2";
    case key::KEY_F3: return "F3";
    case key::KEY_F4: return "F4";
    case key::KEY_F5: return "F5";
    case key::KEY_F6: return "F6";
    case key::KEY_F7: return "F7";
    case key::KEY_F8: return "F8";
    case key::KEY_F9: return "F9";
    case key::KEY_F10: return "F10";
    case key::KEY_F11: return "F11";
    case key::KEY_F12: return "F12";
    case key::KEY_F13: return "F13";
    case key::KEY_F14: return "F14";
    case key::KEY_F15: return "F15";
    case key::KEY_F16: return "F16";
    case key::KEY_F17: return "F17";
    case key::KEY_F18: return "F18";
    case key::KEY_F19: return "F19";
    case key::KEY_F20: return "F20";
    case key::KEY_F21: return "F21";
    case key::KEY_F22: return "F22";
    case key::KEY_F23: return "F23";
    case key::KEY_F24: return "F24";
    case key::KEY_F25: return "F25";
    case key::KEY_F26: return "F26";
    case key::KEY_F27: return "F27";
    case key::KEY_F28: return "F28";
    case key::KEY_F29: return "F29";
    case key::KEY_F30: return "F30";
    case key::KEY_F31: return "F31";
    case key::KEY_F32: return "F32";
    case key::KEY_F33: return "F33";
    case key::KEY_F34: return "F34";
    case key::KEY_F35: return "F35";
    case key::KEY_A: return "a";
    case key::KEY_B: return "b";
    case key::KEY_C: return "c";
    case key::KEY_D: return "d";
    case key::KEY_E: return "e";
    case key::KEY_F: return "f";
    case key::KEY_G: return "g";
    case key::KEY_H: return "h";
    case key::KEY_I: return "i";
    case key::KEY_J: return "j";
    case key::KEY_K: return "k";
    case key::KEY_L: return "l";
    case key::KEY_M: return "m";
    case key::KEY_N: return "n";
    case key::KEY_O: return "o";
    case key::KEY_P: return "p";
    case key::KEY_Q: return "q";
    case key::KEY_R: return "r";
    case key::KEY_S: return "s";
    case key::KEY_T: return "t";
    case key::KEY_U: return "u";
    case key::KEY_V: return "v";
    case key::KEY_W: return "w";
    case key::KEY_X: return "x";
    case key::KEY_Y: return "y";
    case key::KEY_Z: return "z";
    case key::KEY_LEFT: return "left";
    case key::KEY_RIGHT: return "right";
    case key::KEY_UP: return "up";
    case key::KEY_DOWN: return "down";
    case key::KEY_ESCAPE: return "down";
    default: return "unknown";
  }
}

struct keyboard_listener {
  virtual void on_key_pressed(key /*k*/) {}
  virtual void on_key_released(key /*k*/) {}
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
