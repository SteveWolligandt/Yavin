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
