#ifndef YAVIN_KEYBOARD_H
#define YAVIN_KEYBOARD_H
//==============================================================================
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
enum key {
  DIGIT0,
  DIGIT1,
  DIGIT2,
  DIGIT3,
  DIGIT4,
  DIGIT5,
  DIGIT6,
  DIGIT7,
  DIGIT8,
  DIGIT9,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  F25,
  F26,
  F27,
  F28,
  F29,
  F30,
  F31,
  F32,
  F33,
  F34,
  F35,
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  ESCAPE,
  UNKNOWN
};

std::string to_string(key k) {
  switch (k) {
    case key::DIGIT0: return "0";
    case key::DIGIT1: return "1";
    case key::DIGIT2: return "2";
    case key::DIGIT3: return "3";
    case key::DIGIT4: return "4";
    case key::DIGIT5: return "5";
    case key::DIGIT6: return "6";
    case key::DIGIT7: return "7";
    case key::DIGIT8: return "8";
    case key::DIGIT9: return "9";
    case key::F1: return "F1";
    case key::F2: return "F2";
    case key::F3: return "F3";
    case key::F4: return "F4";
    case key::F5: return "F5";
    case key::F6: return "F6";
    case key::F7: return "F7";
    case key::F8: return "F8";
    case key::F9: return "F9";
    case key::F10: return "F10";
    case key::F11: return "F11";
    case key::F12: return "F12";
    case key::F13: return "F13";
    case key::F14: return "F14";
    case key::F15: return "F15";
    case key::F16: return "F16";
    case key::F17: return "F17";
    case key::F18: return "F18";
    case key::F19: return "F19";
    case key::F20: return "F20";
    case key::F21: return "F21";
    case key::F22: return "F22";
    case key::F23: return "F23";
    case key::F24: return "F24";
    case key::F25: return "F25";
    case key::F26: return "F26";
    case key::F27: return "F27";
    case key::F28: return "F28";
    case key::F29: return "F29";
    case key::F30: return "F30";
    case key::F31: return "F31";
    case key::F32: return "F32";
    case key::F33: return "F33";
    case key::F34: return "F34";
    case key::F35: return "F35";
    case key::A: return "a";
    case key::B: return "b";
    case key::C: return "c";
    case key::D: return "d";
    case key::E: return "e";
    case key::F: return "f";
    case key::G: return "g";
    case key::H: return "h";
    case key::I: return "i";
    case key::J: return "j";
    case key::K: return "k";
    case key::L: return "l";
    case key::M: return "m";
    case key::N: return "n";
    case key::O: return "o";
    case key::P: return "p";
    case key::Q: return "q";
    case key::R: return "r";
    case key::S: return "s";
    case key::T: return "t";
    case key::U: return "u";
    case key::V: return "v";
    case key::W: return "w";
    case key::X: return "x";
    case key::Y: return "y";
    case key::Z: return "z";
    case key::LEFT: return "left";
    case key::RIGHT: return "right";
    case key::UP: return "up";
    case key::DOWN: return "down";
    case key::ESCAPE: return "down";
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
