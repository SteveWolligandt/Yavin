#ifndef YAVIN_KEYBOARD_H
#define YAVIN_KEYBOARD_H
//==============================================================================
namespace yavin{
//==============================================================================
enum key {
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
  Z
};

struct keyboard_listener {
  virtual void on_key_pressed(key /*k*/) {}
  virtual void on_key_released(key /*k*/) {}
};

//==============================================================================
}
//==============================================================================
#endif
