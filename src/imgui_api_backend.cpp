#include <yavin/imgui_api_backend.h>
#include <yavin/keyboard.h>
#include <memory>

//==============================================================================
namespace yavin {
//==============================================================================
std::chrono::time_point<std::chrono::system_clock> imgui_api_backend::time =
    std::chrono::system_clock::now();

//==============================================================================
imgui_api_backend::imgui_api_backend() {
  ImGuiIO& io = ImGui::GetIO();
  io.BackendPlatformName = "imgui_impl_yavin";

  io.KeyMap[ImGuiKey_Tab]         = KEY_TAB;
  io.KeyMap[ImGuiKey_LeftArrow]   = KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow]  = KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow]     = KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow]   = KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp]      = KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown]    = KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home]        = KEY_HOME;
  io.KeyMap[ImGuiKey_End]         = KEY_END;
  io.KeyMap[ImGuiKey_Insert]      = KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete]      = KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace]   = KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space]       = KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter]       = KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape]      = KEY_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
  io.KeyMap[ImGuiKey_A]           = KEY_A;
  io.KeyMap[ImGuiKey_C]           = KEY_C;
  io.KeyMap[ImGuiKey_V]           = KEY_V;
  io.KeyMap[ImGuiKey_X]           = KEY_X;
  io.KeyMap[ImGuiKey_Y]           = KEY_Y;
  io.KeyMap[ImGuiKey_Z]           = KEY_Z;
}
//------------------------------------------------------------------------------
imgui_api_backend::~imgui_api_backend() {}
//------------------------------------------------------------------------------
void imgui_api_backend::on_key_pressed(key k) {
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[k] = true;
  io.KeyCtrl     = io.KeysDown[KEY_CTRL_L]  || io.KeysDown[KEY_CTRL_R];
  io.KeyShift    = io.KeysDown[KEY_SHIFT_L] || io.KeysDown[KEY_SHIFT_R];
  io.KeyAlt      = io.KeysDown[KEY_ALT_L]   || io.KeysDown[KEY_ALT_R];
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_key_released(key k) {
  ImGuiIO& io    = ImGui::GetIO();
  io.KeysDown[k] = false;
  io.KeyCtrl     = io.KeysDown[KEY_CTRL_L]  || io.KeysDown[KEY_CTRL_R];
  io.KeyShift    = io.KeysDown[KEY_SHIFT_L] || io.KeysDown[KEY_SHIFT_R];
  io.KeyAlt      = io.KeysDown[KEY_ALT_L]   || io.KeysDown[KEY_ALT_R];
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_button_pressed(button b) {
  ImGuiIO& io = ImGui::GetIO();
  switch(b){
    case BUTTON_LEFT: io.MouseDown[0] = true; break;
    case BUTTON_RIGHT: io.MouseDown[1] = true; break;
    case BUTTON_MIDDLE: io.MouseDown[2] = true; break;
    case BUTTON_UNKNOWN:
    default: break;
  }
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_button_released(button b) {
  ImGuiIO& io = ImGui::GetIO();
  switch (b) {
    case BUTTON_LEFT: io.MouseDown[0] = false; break;
    case BUTTON_RIGHT: io.MouseDown[1] = false; break;
    case BUTTON_MIDDLE: io.MouseDown[2] = false; break;
    case BUTTON_UNKNOWN:
    default: break;
  }
}
//------------------------------------------------------------------------------
imgui_api_backend& imgui_api_backend::instance() {
  static auto inst = std::make_unique<imgui_api_backend>();
  return *inst;
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_resize(int w, int h) {
  ImGuiIO& io    = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)w, (float)h);
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_mouse_motion(int x, int y) {
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2((float)x, (float)y);
}
//------------------------------------------------------------------------------
void imgui_api_backend::new_frame() {
  // Setup time step
  ImGuiIO& io            = ImGui::GetIO();
  auto      current_time  = std::chrono::system_clock::now();
  int       delta_time_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time - time)
          .count();
  if (delta_time_ms <= 0) delta_time_ms = 1;
  io.DeltaTime = delta_time_ms / 1000.0;

  time = current_time;
  // Start the frame
  ImGui::NewFrame();
}
//------------------------------------------------------------------------------
void imgui_api_backend::on_mouse_wheel(int dir) {
  ImGuiIO& io = ImGui::GetIO();
  if (dir > 0) {
    io.MouseWheel += 1.0;
  } else if (dir < 0) {
    io.MouseWheel -= 1.0;
  }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
