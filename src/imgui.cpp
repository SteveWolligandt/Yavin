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
  io.KeyMap[ImGuiKey_Tab]         = '\t';  // == 9 == CTRL+I
  io.KeyMap[ImGuiKey_LeftArrow]   = key::KEY_LEFT;
  io.KeyMap[ImGuiKey_RightArrow]  = key::KEY_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow]     = key::KEY_UP;
  io.KeyMap[ImGuiKey_DownArrow]   = key::KEY_DOWN;
  io.KeyMap[ImGuiKey_PageUp]      = key::KEY_PAGE_UP;
  io.KeyMap[ImGuiKey_PageDown]    = key::KEY_PAGE_DOWN;
  io.KeyMap[ImGuiKey_Home]        = key::KEY_HOME;
  io.KeyMap[ImGuiKey_End]         = key::KEY_END;
  io.KeyMap[ImGuiKey_Insert]      = key::KEY_INSERT;
  io.KeyMap[ImGuiKey_Delete]      = key::KEY_DELETE;
  io.KeyMap[ImGuiKey_Backspace]   = key::KEY_BACKSPACE;
  io.KeyMap[ImGuiKey_Space]       = key::KEY_SPACE;
  io.KeyMap[ImGuiKey_Enter]       = key::KEY_ENTER;
  io.KeyMap[ImGuiKey_Escape]      = key::KEY_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = key::KEY_KP_ENTER;
  io.KeyMap[ImGuiKey_A]           = key::KEY_A;
  io.KeyMap[ImGuiKey_C]           = key::KEY_C;
  io.KeyMap[ImGuiKey_V]           = key::KEY_V;
  io.KeyMap[ImGuiKey_X]           = key::KEY_X;
  io.KeyMap[ImGuiKey_Y]           = key::KEY_Y;
  io.KeyMap[ImGuiKey_Z]           = key::KEY_Z;
}
//------------------------------------------------------------------------------
imgui::~imgui() {}
//------------------------------------------------------------------------------
void imgui::on_key_pressed(key k) {
  //// Send character to imgui
  //// printf("char_down_func %d '%c'\n", c, c);
  //ImGuiIO& io = ImGui::GetIO();
  //if (c >= 32) io.AddInputCharacter((unsigned int)c);
  //
  //// Store letters in KeysDown[] array as both uppercase and lowercase + Handle
  //// GLUT translating CTRL+A..CTRL+Z as 1..26. This is a hacky mess but GLUT is
  //// unable to distinguish e.g. a TAB key from CTRL+I so this is probably the
  //// best we can do here.
  //if (c >= 1 && c <= 26)
  //  io.KeysDown[c] = io.KeysDown[c - 1 + 'a'] = io.KeysDown[c - 1 + 'A'] = true;
  //else if (c >= 'a' && c <= 'z')
  //  io.KeysDown[c] = io.KeysDown[c - 'a' + 'A'] = true;
  //else if (c >= 'A' && c <= 'Z')
  //  io.KeysDown[c] = io.KeysDown[c - 'A' + 'a'] = true;
  //else
  //  io.KeysDown[c] = true;
}
//------------------------------------------------------------------------------
void imgui::on_key_released(key k) {
  //// printf("char_up_func %d '%c'\n", c, c);
  //ImGuiIO& io = ImGui::GetIO();
  //if (c >= 1 && c <= 26)
  //  io.KeysDown[c] = io.KeysDown[c - 1 + 'a'] = io.KeysDown[c - 1 + 'A'] =
  //      false;
  //else if (c >= 'a' && c <= 'z')
  //  io.KeysDown[c] = io.KeysDown[c - 'a' + 'A'] = false;
  //else if (c >= 'A' && c <= 'Z')
  //  io.KeysDown[c] = io.KeysDown[c - 'A' + 'a'] = false;
  //else
  //  io.KeysDown[c] = false;
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
