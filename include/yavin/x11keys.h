#ifndef YAVIN_X11KEYS_H
#define YAVIN_X11KEYS_H
#include <X11/XKBlib.h>
//==============================================================================
namespace yavin {
//==============================================================================
auto x11_keysym_to_key(KeySym keysym) {
  switch (keysym) {
    case XK_0: return key::KEY_0;
    case XK_1: return key::KEY_1;
    case XK_2: return key::KEY_2;
    case XK_3: return key::KEY_3;
    case XK_4: return key::KEY_4;
    case XK_5: return key::KEY_5;
    case XK_6: return key::KEY_6;
    case XK_7: return key::KEY_7;
    case XK_8: return key::KEY_8;
    case XK_9: return key::KEY_9;
    case XK_F1: return key::KEY_F1;
    case XK_F2: return key::KEY_F2;
    case XK_F3: return key::KEY_F3;
    case XK_F4: return key::KEY_F4;
    case XK_F5: return key::KEY_F5;
    case XK_F6: return key::KEY_F6;
    case XK_F7: return key::KEY_F7;
    case XK_F8: return key::KEY_F8;
    case XK_F9: return key::KEY_F9;
    case XK_F10: return key::KEY_F10;
    case XK_F11: return key::KEY_F11;
    case XK_F12: return key::KEY_F12;
    case XK_F13: return key::KEY_F13;
    case XK_F14: return key::KEY_F14;
    case XK_F15: return key::KEY_F15;
    case XK_F16: return key::KEY_F16;
    case XK_F17: return key::KEY_F17;
    case XK_F18: return key::KEY_F18;
    case XK_F19: return key::KEY_F19;
    case XK_F20: return key::KEY_F20;
    case XK_F21: return key::KEY_F21;
    case XK_F22: return key::KEY_F22;
    case XK_F23: return key::KEY_F23;
    case XK_F24: return key::KEY_F24;
    case XK_F25: return key::KEY_F25;
    case XK_F26: return key::KEY_F26;
    case XK_F27: return key::KEY_F27;
    case XK_F28: return key::KEY_F28;
    case XK_F29: return key::KEY_F29;
    case XK_F30: return key::KEY_F30;
    case XK_F31: return key::KEY_F31;
    case XK_F32: return key::KEY_F32;
    case XK_F33: return key::KEY_F33;
    case XK_F34: return key::KEY_F34;
    case XK_F35: return key::KEY_F35;
    case XK_a: return key::KEY_A;
    case XK_b: return key::KEY_B;
    case XK_c: return key::KEY_C;
    case XK_d: return key::KEY_D;
    case XK_e: return key::KEY_E;
    case XK_f: return key::KEY_F;
    case XK_g: return key::KEY_G;
    case XK_h: return key::KEY_H;
    case XK_i: return key::KEY_I;
    case XK_j: return key::KEY_J;
    case XK_k: return key::KEY_K;
    case XK_l: return key::KEY_L;
    case XK_m: return key::KEY_M;
    case XK_n: return key::KEY_N;
    case XK_o: return key::KEY_O;
    case XK_p: return key::KEY_P;
    case XK_q: return key::KEY_Q;
    case XK_r: return key::KEY_R;
    case XK_s: return key::KEY_S;
    case XK_t: return key::KEY_T;
    case XK_u: return key::KEY_U;
    case XK_v: return key::KEY_V;
    case XK_w: return key::KEY_W;
    case XK_x: return key::KEY_X;
    case XK_y: return key::KEY_Y;
    case XK_z: return key::KEY_Z;
    case XK_BackSpace: return key::KEY_BACKSPACE;
    case XK_Insert: return key::KEY_INSERT;
    case XK_Home: return key::KEY_HOME;
    case XK_Page_Up: return key::KEY_PAGE_UP;
    case XK_Page_Down: return key::KEY_PAGE_DOWN;
    case XK_Delete: return key::KEY_DELETE;
    case XK_End: return key::KEY_END;
    case XK_Tab: return key::KEY_TAB;
    case XK_Return: return key::KEY_ENTER;
    case XK_KP_Enter: return key::KEY_KP_ENTER;
    case XK_space: return key::KEY_SPACE;
    case XK_Left: return key::KEY_LEFT;
    case XK_Up: return key::KEY_UP;
    case XK_Right: return key::KEY_RIGHT;
    case XK_Down: return key::KEY_DOWN;
    case XK_Escape: return key::KEY_ESCAPE;
    case XK_Alt_R: return KEY_ALT_R;
    case XK_Alt_L: return KEY_ALT_L;
    case XK_Shift_R: return KEY_SHIFT_R;
    case XK_Shift_L: return KEY_SHIFT_L;
    case XK_Control_R: return KEY_CTRL_R;
    case XK_Control_L: return KEY_CTRL_L;
    default: return key::KEY_UNKNOWN;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
