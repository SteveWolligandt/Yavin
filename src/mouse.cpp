#include <yavin/mouse.h>
//==============================================================================
namespace yavin {
//==============================================================================
std::string to_string(button b) {
  switch (b) {
    case button::BUTTON_LEFT: return "left";
    case button::BUTTON_RIGHT: return "right";
    case button::BUTTON_MIDDLE: return "middle";
    default: return "unknown";
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
