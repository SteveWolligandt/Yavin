#ifndef YAVIN_X11BUTTONS_H
#define YAVIN_X11BUTTONS_H
//==============================================================================
#include <yavin/mouse.h>
//==============================================================================
namespace yavin {
//==============================================================================
inline auto x11_button_to_button(unsigned int button) {
  switch (button) {
    case 1: return button::BUTTON_LEFT;
    case 2: return button::BUTTON_MIDDLE;
    case 3: return button::BUTTON_RIGHT;
    case 4: return button::BUTTON_WHEEL_UP;
    case 5: return button::BUTTON_WHEEL_DOWN;
    case 6: return button::BUTTON_WHEEL_RIGHT;
    case 7: return button::BUTTON_WHEEL_LEFT;
    default: return button::BUTTON_UNKNOWN;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif

