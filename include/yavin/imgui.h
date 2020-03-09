#ifndef YAVIN_IMGUI_H
#define YAVIN_IMGUI_H
//==============================================================================
#include <yavin/glincludes.h>

#include <imgui/imgui.h>
#include <chrono>
#include "glx_window.h"
#include "window_listener.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct imgui : window_listener {
  static imgui& instance();
  static std::chrono::time_point<std::chrono::system_clock> time;
  //----------------------------------------------------------------------------
  imgui();
  ~imgui();
  //----------------------------------------------------------------------------
  void on_key_pressed(key k) override;
  void on_key_released(key k) override;
  void on_button_pressed(button b) override;
  void on_button_released(button b) override;
  void on_mouse_motion(int x, int y) override;
  void on_resize(int width, int height) override;
  void new_frame();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
