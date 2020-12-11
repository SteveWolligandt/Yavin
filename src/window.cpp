#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <yavin/context.h>
#include <yavin/window.h>

#include <cstring>
#include <string>
//==============================================================================
namespace yavin {
//==============================================================================
window::window(const std::string &title, size_t width, size_t height) {
  setup(title, width, height);
}
//------------------------------------------------------------------------------
window::~window() {
  for (auto &t : m_async_tasks) { t.join(); }
  deinit_imgui();
}
//==============================================================================
// methods
//==============================================================================
context window::create_shared_context() const {
  return context{*this};
}
//------------------------------------------------------------------------------
void window::make_current() {
  if (!eglMakeCurrent(m_egl_disp->get(), m_egl_surface->get(),
                      m_egl_surface->get(), m_egl_context->get())) {
    throw std::runtime_error{"[EGL] cannot make window current. " +
                             egl::error_string(eglGetError())};
  }
  if (m_glew == nullptr) { m_glew = glew::create(); }
}
//------------------------------------------------------------------------------
void window::release() {
  if (!eglMakeCurrent(m_egl_disp->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release window"};
  }
}
//------------------------------------------------------------------------------
void window::refresh() {
  check_events();
  imgui_api_backend::instance().new_frame();
}
//------------------------------------------------------------------------------
void window::check_events() {
  {
    std::lock_guard lock{m_async_tasks_mutex};
    if (!m_joinable_async_tasks.empty()) {
      for (auto const it : m_joinable_async_tasks) {
        it->join();
        m_async_tasks.erase(it);
      }
      m_joinable_async_tasks.clear();
    }
  }
  m_egl_surface->x11_window()->check_events();
}
//------------------------------------------------------------------------------
void window::render_imgui() {
  ImGui::Render();
  m_imgui_render_backend->render_draw_data(ImGui::GetDrawData());
}
//------------------------------------------------------------------------------
void window::setup(const std::string &title, size_t width, size_t height) {
  auto x11_disp = std::make_shared<x11::display>();
  m_egl_disp     = std::make_shared<egl::display>(x11_disp);
  // EGL context and surface
  eglBindAPI(EGL_OPENGL_API);
  EGLint const ctx_attrs[] = {EGL_CONTEXT_MAJOR_VERSION, 4,
                              EGL_CONTEXT_MINOR_VERSION, 5,
                              EGL_NONE};
  EGLint const cfg_attrs[] = {EGL_RED_SIZE,     8,
                              EGL_GREEN_SIZE,   8,
                              EGL_BLUE_SIZE,    8,
                              EGL_ALPHA_SIZE,   8,
                              EGL_DEPTH_SIZE,   24,
                              EGL_CONFORMANT,   EGL_OPENGL_BIT,
                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                              EGL_NONE};
  auto [visual_info, cfg] =
      m_egl_disp->create_visual_info(cfg_attrs, x11_disp);
  m_egl_context =
      std::make_shared<egl::context>(m_egl_disp, cfg, ctx_attrs);
  m_egl_surface = std::make_shared<egl::surface>(
      title, width, height, m_egl_disp, x11_disp, cfg, visual_info);
  m_egl_surface->x11_window()->add_listener(*this);
  make_current();

  init_imgui(width, height);
  XFree(visual_info);
}
//------------------------------------------------------------------------------
void window::swap_buffers() {
  std::lock_guard lock{m_egl_mutex};
  if (!eglSwapBuffers(m_egl_disp->get(), m_egl_surface->get())) {
    auto err = eglGetError();
    throw std::runtime_error{"[EGL] eglSwapBuffers - " +
                             egl::error_string(err)};
  }
}
//------------------------------------------------------------------------------
void window::init_imgui(size_t width, size_t height) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  m_imgui_render_backend = std::make_unique<struct imgui_render_backend>();
  imgui_api_backend::instance().on_resize(width, height);

  ImVec4 *colors                         = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
  colors[ImGuiCol_ChildBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
  colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_Border]                = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
  colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
  colors[ImGuiCol_FrameBgActive]         = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
  colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_TitleBgActive]         = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark]             = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
  colors[ImGuiCol_SliderGrab]            = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
  colors[ImGuiCol_Button]                = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
  colors[ImGuiCol_ButtonHovered]         = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
  colors[ImGuiCol_ButtonActive]          = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
  colors[ImGuiCol_Header]                = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
  colors[ImGuiCol_HeaderHovered]         = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
  colors[ImGuiCol_HeaderActive]          = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
  colors[ImGuiCol_Separator]             = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
  colors[ImGuiCol_SeparatorActive]       = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_ResizeGrip]            = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
  colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
  colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
  colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram]         = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
  colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
  colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
}
//------------------------------------------------------------------------------
void window::deinit_imgui() {
  ImGui::DestroyContext();
}
//------------------------------------------------------------------------------
void window::on_key_pressed(key k) {
  imgui_api_backend::instance().on_key_pressed(k);
  if (!ImGui::GetIO().WantCaptureKeyboard) { notify_key_pressed(k); }
}
//------------------------------------------------------------------------------
void window::on_key_released(key k) {
  imgui_api_backend::instance().on_key_released(k);
  if (!ImGui::GetIO().WantCaptureKeyboard) { notify_key_released(k); }
}
//------------------------------------------------------------------------------
void window::on_button_pressed(button b) {
  imgui_api_backend::instance().on_button_pressed(b);
  if (!ImGui::GetIO().WantCaptureMouse) { notify_button_pressed(b); }
}
//------------------------------------------------------------------------------
void window::on_button_released(button b) {
  imgui_api_backend::instance().on_button_released(b);
  if (!ImGui::GetIO().WantCaptureMouse) { notify_button_released(b); }
}
//------------------------------------------------------------------------------
void window::on_wheel_up() {
  imgui_api_backend::instance().on_mouse_wheel(1);
  if (!ImGui::GetIO().WantCaptureMouse) { notify_wheel_up(); }
}
//------------------------------------------------------------------------------
void window::on_wheel_down() {
  imgui_api_backend::instance().on_mouse_wheel(-1);
  if (!ImGui::GetIO().WantCaptureMouse) { notify_wheel_down(); }
}
//------------------------------------------------------------------------------
void window::on_wheel_left() {
  if (!ImGui::GetIO().WantCaptureMouse) { notify_wheel_left(); }
}
//------------------------------------------------------------------------------
void window::on_wheel_right() {
  if (!ImGui::GetIO().WantCaptureMouse) { notify_wheel_right(); }
}
//------------------------------------------------------------------------------
void window::on_mouse_motion(int x, int y) {
  imgui_api_backend::instance().on_mouse_motion(x,y);
  if (!ImGui::GetIO().WantCaptureMouse) { notify_mouse_motion(x, y); }
}
//------------------------------------------------------------------------------
void window::on_resize(int w, int h) {
  imgui_api_backend::instance().on_resize(w, h);
  notify_resize(w, h);
}
//==============================================================================
}  // namespace yavin
//==============================================================================
