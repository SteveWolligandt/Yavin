#ifndef YAVIN_X11BUTTONS_H
#define YAVIN_X11BUTTONS_H
//==============================================================================
namespace yavin {
//==============================================================================
auto x11_button_to_button(unsigned int button) {
  switch (button) {
    case 1: return button::BUTTON_LEFT;
    case 2: return button::BUTTON_MIDDLE;
    case 3: return button::BUTTON_RIGHT;
    default: return button::BUTTON_UNKNOWN;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif

