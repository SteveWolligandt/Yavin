#ifndef YAVIN_X11KEYS_H
#define YAVIN_X11KEYS_H
//==============================================================================
#include <X11/XKBlib.h>
#ifdef Bool
#undef Bool
#endif
#include <yavin/keyboard.h>
//==============================================================================
namespace yavin {
//==============================================================================
inline auto x11_keysym_to_key(KeySym keysym) {
  switch (keysym) {
    case XK_0: return KEY_0;
    case XK_1: return KEY_1;
    case XK_2: return KEY_2;
    case XK_3: return KEY_3;
    case XK_4: return KEY_4;
    case XK_5: return KEY_5;
    case XK_6: return KEY_6;
    case XK_7: return KEY_7;
    case XK_8: return KEY_8;
    case XK_9: return KEY_9;
    case XK_F1: return KEY_F1;
    case XK_F2: return KEY_F2;
    case XK_F3: return KEY_F3;
    case XK_F4: return KEY_F4;
    case XK_F5: return KEY_F5;
    case XK_F6: return KEY_F6;
    case XK_F7: return KEY_F7;
    case XK_F8: return KEY_F8;
    case XK_F9: return KEY_F9;
    case XK_F10: return KEY_F10;
    case XK_F11: return KEY_F11;
    case XK_F12: return KEY_F12;
    case XK_F13: return KEY_F13;
    case XK_F14: return KEY_F14;
    case XK_F15: return KEY_F15;
    case XK_F16: return KEY_F16;
    case XK_F17: return KEY_F17;
    case XK_F18: return KEY_F18;
    case XK_F19: return KEY_F19;
    case XK_F20: return KEY_F20;
    case XK_F21: return KEY_F21;
    case XK_F22: return KEY_F22;
    case XK_F23: return KEY_F23;
    case XK_F24: return KEY_F24;
    case XK_F25: return KEY_F25;
    case XK_F26: return KEY_F26;
    case XK_F27: return KEY_F27;
    case XK_F28: return KEY_F28;
    case XK_F29: return KEY_F29;
    case XK_F30: return KEY_F30;
    case XK_F31: return KEY_F31;
    case XK_F32: return KEY_F32;
    case XK_F33: return KEY_F33;
    case XK_F34: return KEY_F34;
    case XK_F35: return KEY_F35;
    case XK_a: return KEY_A;
    case XK_b: return KEY_B;
    case XK_c: return KEY_C;
    case XK_d: return KEY_D;
    case XK_e: return KEY_E;
    case XK_f: return KEY_F;
    case XK_g: return KEY_G;
    case XK_h: return KEY_H;
    case XK_i: return KEY_I;
    case XK_j: return KEY_J;
    case XK_k: return KEY_K;
    case XK_l: return KEY_L;
    case XK_m: return KEY_M;
    case XK_n: return KEY_N;
    case XK_o: return KEY_O;
    case XK_p: return KEY_P;
    case XK_q: return KEY_Q;
    case XK_r: return KEY_R;
    case XK_s: return KEY_S;
    case XK_t: return KEY_T;
    case XK_u: return KEY_U;
    case XK_v: return KEY_V;
    case XK_w: return KEY_W;
    case XK_x: return KEY_X;
    case XK_y: return KEY_Y;
    case XK_z: return KEY_Z;
    case XK_BackSpace: return KEY_BACKSPACE;
    case XK_Insert: return KEY_INSERT;
    case XK_Home: return KEY_HOME;
    case XK_Page_Up: return KEY_PAGE_UP;
    case XK_Page_Down: return KEY_PAGE_DOWN;
    case XK_Delete: return KEY_DELETE;
    case XK_End: return KEY_END;
    case XK_Tab: return KEY_TAB;
    case XK_Return: return KEY_ENTER;
    case XK_KP_Enter: return KEY_KP_ENTER;
    case XK_space: return KEY_SPACE;
    case XK_Left: return KEY_LEFT;
    case XK_Up: return KEY_UP;
    case XK_Right: return KEY_RIGHT;
    case XK_Down: return KEY_DOWN;
    case XK_Escape: return KEY_ESCAPE;
    case XK_Alt_R: return KEY_ALT_R;
    case XK_Alt_L: return KEY_ALT_L;
    case XK_Shift_R: return KEY_SHIFT_R;
    case XK_Shift_L: return KEY_SHIFT_L;
    case XK_Control_R: return KEY_CTRL_R;
    case XK_Control_L: return KEY_CTRL_L;
    default: return KEY_UNKNOWN;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
