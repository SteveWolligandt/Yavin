#ifndef YAVIN_IMGUI_API_BACKEND_H
#define YAVIN_IMGUI_API_BACKEND_H
//==============================================================================
#include <yavin/glincludes.h>

#include <imgui/imgui.h>
#include <chrono>
#include "glx_window.h"
#include "window_listener.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct imgui_api_backend : window_listener {
  static imgui_api_backend& instance();
  static std::chrono::time_point<std::chrono::system_clock> time;
  //----------------------------------------------------------------------------
  imgui_api_backend();
  ~imgui_api_backend();
  //----------------------------------------------------------------------------
  void on_key_pressed(key k) override;
  void on_key_released(key k) override;
  void on_button_pressed(button b) override;
  void on_button_released(button b) override;
  void on_mouse_motion(int x, int y) override;
  void on_resize(int width, int height) override;
  /// TODO integrate
  void on_mouse_wheel(int dir);
  void new_frame();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
