#include <yavin/imgui.h>
#include <yavin/keyboard.h>
#include <memory>

//==============================================================================
namespace yavin {
//==============================================================================
std::chrono::time_point<std::chrono::system_clock> imgui::time =
    std::chrono::system_clock::now();

//==============================================================================
imgui::imgui() {
  ImGuiIO& io = ImGui::GetIO();
  io.BackendPlatformName = "imgui_impl_yavin";

  // Glut has 1 function for characters and one for "special keys". We map the
  // characters in the 0..255 range and the keys above.
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
imgui::~imgui() {}
//------------------------------------------------------------------------------
void imgui::on_key_pressed(key k) {
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[k]         = true;
  io.KeyCtrl             = io.KeysDown[KEY_CTRL_L] || io.KeysDown[KEY_CTRL_R];
  io.KeyShift = io.KeysDown[KEY_SHIFT_L] || io.KeysDown[KEY_SHIFT_R];
  io.KeyAlt = io.KeysDown[KEY_ALT_L] || io.KeysDown[KEY_ALT_R];
}
//------------------------------------------------------------------------------
void imgui::on_key_released(key k) {
  ImGuiIO&   io = ImGui::GetIO();
  io.KeysDown[k]         = true;
  io.KeyCtrl             = io.KeysDown[KEY_CTRL_L] || io.KeysDown[KEY_CTRL_R];
  io.KeyShift = io.KeysDown[KEY_SHIFT_L] || io.KeysDown[KEY_SHIFT_R];
  io.KeyAlt = io.KeysDown[KEY_ALT_L] || io.KeysDown[KEY_ALT_R];
}
//------------------------------------------------------------------------------
void imgui::on_button_pressed(button b) {
  ImGuiIO& io = ImGui::GetIO();
  switch(b){
    case BUTTON_LEFT: io.MouseDown[0] = true; break;
    case BUTTON_RIGHT: io.MouseDown[1] = true; break;
    case BUTTON_MIDDLE: io.MouseDown[2] = true; break;
  }
}
//------------------------------------------------------------------------------
void imgui::on_button_released(button b) {
  ImGuiIO& io = ImGui::GetIO();
  switch(b){
    case BUTTON_LEFT: io.MouseDown[0] = false; break;
    case BUTTON_RIGHT: io.MouseDown[1] = false; break;
    case BUTTON_MIDDLE: io.MouseDown[2] = false; break;
  }
}
//------------------------------------------------------------------------------
imgui& imgui::instance() {
  static auto inst = std::make_unique<imgui>();
  return *inst;
}
//------------------------------------------------------------------------------
void imgui::on_resize(int w, int h) {
  ImGuiIO& io    = ImGui::GetIO();
  io.DisplaySize = ImVec2((float)w, (float)h);
}
//------------------------------------------------------------------------------
void imgui::on_mouse_motion(int x, int y) {
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2((float)x, (float)y);
}
//------------------------------------------------------------------------------
void imgui::new_frame() {
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
////------------------------------------------------------------------------------
//void imgui::install_funcs() {
//  glutReshapeFunc(ImGui_ImplGLUT_ReshapeFunc);
//  glutMotionFunc(ImGui_ImplGLUT_MotionFunc);
//  glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
//  glutMouseFunc(ImGui_ImplGLUT_MouseFunc);
//  glutMouseWheelFunc(ImGui_ImplGLUT_MouseWheelFunc);
//  glutKeyboardFunc(ImGui_ImplGLUT_KeyboardFunc);
//  glutKeyboardUpFunc(ImGui_ImplGLUT_KeyboardUpFunc);
//  glutSpecialFunc(ImGui_ImplGLUT_SpecialFunc);
//  glutSpecialUpFunc(ImGui_ImplGLUT_SpecialUpFunc);
//}
////------------------------------------------------------------------------------
//void imgui::special_func(int key, int x, int y) {
//  // printf("key_down_func %d\n", key);
//  ImGuiIO& io = ImGui::GetIO();
//  if (key + 256 < IM_ARRAYSIZE(io.KeysDown)) io.KeysDown[key + 256] = true;
//  (void)x;
//  (void)y;  // Unused
//}
////------------------------------------------------------------------------------
//void imgui::special_up_func(int key, int x, int y) {
//  // printf("key_up_func %d\n", key);
//  ImGuiIO& io = ImGui::GetIO();
//  if (key + 256 < IM_ARRAYSIZE(io.KeysDown)) io.KeysDown[key + 256] = false;
//  update_keyboard_mods();
//  (void)x;
//  (void)y;  // Unused
//}
////------------------------------------------------------------------------------
//void imgui::mouse_wheel_func(int button, int dir, int x, int y) {
//  //ImGuiIO& io = ImGui::GetIO();
//  //io.MousePos = ImVec2((float)x, (float)y);
//  //if (dir > 0)
//  //  io.MouseWheel += 1.0;
//  //else if (dir < 0)
//  //  io.MouseWheel -= 1.0;
//  //(void)button;  // Unused
//}
//==============================================================================
}  // namespace yavin
//==============================================================================
