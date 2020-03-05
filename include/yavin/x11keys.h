#ifndef YAVIN_X11KEYS_H
#define YAVIN_X11KEYS_H
#include <X11/XKBlib.h>
//==============================================================================
namespace yavin {
//==============================================================================
auto x11_keysym_to_key(KeySym keysym) {
  switch (keysym) {
    case XK_0: return key::DIGIT0;
    case XK_1: return key::DIGIT1;
    case XK_2: return key::DIGIT2;
    case XK_3: return key::DIGIT3;
    case XK_4: return key::DIGIT4;
    case XK_5: return key::DIGIT5;
    case XK_6: return key::DIGIT6;
    case XK_7: return key::DIGIT7;
    case XK_8: return key::DIGIT8;
    case XK_9: return key::DIGIT9;
    case XK_F1: return key::F1;
    case XK_F2: return key::F2;
    case XK_F3: return key::F3;
    case XK_F4: return key::F4;
    case XK_F5: return key::F5;
    case XK_F6: return key::F6;
    case XK_F7: return key::F7;
    case XK_F8: return key::F8;
    case XK_F9: return key::F9;
    case XK_F10: return key::F10;
    case XK_F11: return key::F11;
    case XK_F12: return key::F12;
    case XK_F13: return key::F13;
    case XK_F14: return key::F14;
    case XK_F15: return key::F15;
    case XK_F16: return key::F16;
    case XK_F17: return key::F17;
    case XK_F18: return key::F18;
    case XK_F19: return key::F19;
    case XK_F20: return key::F20;
    case XK_F21: return key::F21;
    case XK_F22: return key::F22;
    case XK_F23: return key::F23;
    case XK_F24: return key::F24;
    case XK_F25: return key::F25;
    case XK_F26: return key::F26;
    case XK_F27: return key::F27;
    case XK_F28: return key::F28;
    case XK_F29: return key::F29;
    case XK_F30: return key::F30;
    case XK_F31: return key::F31;
    case XK_F32: return key::F32;
    case XK_F33: return key::F33;
    case XK_F34: return key::F34;
    case XK_F35: return key::F35;
    case XK_a: return key::A;
    case XK_b: return key::B;
    case XK_c: return key::C;
    case XK_d: return key::D;
    case XK_e: return key::E;
    case XK_f: return key::F;
    case XK_g: return key::G;
    case XK_h: return key::H;
    case XK_i: return key::I;
    case XK_j: return key::J;
    case XK_k: return key::K;
    case XK_l: return key::L;
    case XK_m: return key::M;
    case XK_n: return key::N;
    case XK_o: return key::O;
    case XK_p: return key::P;
    case XK_q: return key::Q;
    case XK_r: return key::R;
    case XK_s: return key::S;
    case XK_t: return key::T;
    case XK_u: return key::U;
    case XK_v: return key::V;
    case XK_w: return key::W;
    case XK_x: return key::X;
    case XK_y: return key::Y;
    case XK_z: return key::Z;
    case XK_Left: return key::LEFT;
    case XK_Up: return key::UP;
    case XK_Right: return key::RIGHT;
    case XK_Down: return key::DOWN;
    case XK_Escape: return key::ESCAPE;
    default: return key::UNKNOWN;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
