#ifndef YAVIN_IMGUI_API_BACKEND_H
#define YAVIN_IMGUI_API_BACKEND_H
//==============================================================================
#include <yavin/glincludes.h>
#include <yavin/imgui_includes.h>
#include <chrono>
#include "window_listener.h"
//==============================================================================
namespace yavin {
//==============================================================================
struct imgui_api_backend  {
  static imgui_api_backend& instance();
  static std::chrono::time_point<std::chrono::system_clock> time;
  //----------------------------------------------------------------------------
  imgui_api_backend();
  virtual ~imgui_api_backend();
  //----------------------------------------------------------------------------
  void on_key_pressed(key k) ;
  void on_key_released(key k) ;
  void on_button_pressed(button b) ;
  void on_button_released(button b) ;
  void on_mouse_motion(int x, int y) ;
  void on_resize(int width, int height) ;
  void on_mouse_wheel(int dir);
  void new_frame();
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
