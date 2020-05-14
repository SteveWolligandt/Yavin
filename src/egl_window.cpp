#define YAVIN_X11_CONTEXT_DONT_DELETE
#include <yavin/egl_window.h>
#include <yavin/x11buttons.h>
#include <yavin/x11keys.h>

#include <cstring>
#include <string>

//==============================================================================
namespace yavin {
//==============================================================================
std::string egl_error_to_string(EGLint error) {
  switch (error) {
    case EGL_SUCCESS: return "No error";
    case EGL_NOT_INITIALIZED:
      return "EGL not initialized or failed to initialize";
    case EGL_BAD_ACCESS: return "Resource inaccessible";
    case EGL_BAD_ALLOC: return "Cannot allocate resources";
    case EGL_BAD_ATTRIBUTE: return "Unrecognized attribute or attribute value";
    case EGL_BAD_CONTEXT: return "Invalid EGL context";
    case EGL_BAD_CONFIG: return "Invalid EGL frame buffer configuration";
    case EGL_BAD_CURRENT_SURFACE: return "Current surface is no longer valid";
    case EGL_BAD_DISPLAY: return "Invalid EGL display";
    case EGL_BAD_SURFACE: return "Invalid surface";
    case EGL_BAD_MATCH: return "Inconsistent arguments";
    case EGL_BAD_PARAMETER: return "Invalid argument";
    case EGL_BAD_NATIVE_PIXMAP: return "Invalid native pixmap";
    case EGL_BAD_NATIVE_WINDOW: return "Invalid native window";
    case EGL_CONTEXT_LOST: return "Context lost";
    default: return "unknown error";
  }
}
//==============================================================================
window::window(const std::string &title, GLsizei width, GLsizei height,
               EGLint major, EGLint minor)
    : m_x_display{XOpenDisplay(nullptr)},
      m_egl_display{
#ifdef USE_EGL_KHR_PLATFORM_X11
          eglGetPlatformDisplay(EGL_PLATFORM_X11_KHR, m_x_display, nullptr)
#else
          eglGetDisplay(m_x_display)
#endif
      } {
  setup(title, width, height, major, minor);
}
//------------------------------------------------------------------------------
window::~window() {
  deinit_imgui();
  eglDestroyContext(m_egl_display, m_egl_context);
  eglDestroySurface(m_egl_display, m_egl_surface);
  eglTerminate(m_egl_display);
  XDestroyWindow(m_x_display, m_x_window);
  XCloseDisplay(m_x_display);
}
//==============================================================================
// methods
//==============================================================================
// context window::create_shared_context() const {
//  return context{*this};
//}
//------------------------------------------------------------------------------
void window::make_current() {
  if (!eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface,
                      m_egl_context)) {
    throw std::runtime_error{"[EGL] cannot make window current."};
  }
  if (!m_glew_initialized) {
    init_glew();
    m_glew_initialized = true;
  }
}
//------------------------------------------------------------------------------
void window::release() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release window"};
  }
}
//------------------------------------------------------------------------------
void window::refresh() {
  check_events();
  imgui_api_backend::instance().new_frame();
}
//------------------------------------------------------------------------------
void window::render_imgui() {
  ImGui::Render();
  m_imgui_render_backend->render_draw_data(ImGui::GetDrawData());
}
//------------------------------------------------------------------------------
void window::check_events() {
  while (XCheckWindowEvent(m_x_display, m_x_window,
                           ExposureMask | KeyPressMask | ButtonPress |
                               StructureNotifyMask | ButtonReleaseMask |
                               KeyReleaseMask | EnterWindowMask |
                               LeaveWindowMask | PointerMotionMask |
                               Button1MotionMask | VisibilityChangeMask,
                           &m_xevent)) {
    const auto k = x11_keysym_to_key(
        XkbKeycodeToKeysym(m_x_display, m_xevent.xkey.keycode, 0, 0));
    const auto b = x11_button_to_button(m_xevent.xbutton.button);
    switch (m_xevent.type) {
      case KeyPress:
        imgui_api_backend::instance().on_key_pressed(k);
        if (!ImGui::GetIO().WantCaptureKeyboard) { notify_key_pressed(k); }
        break;
      case KeyRelease:
        imgui_api_backend::instance().on_key_released(k);
        if (!ImGui::GetIO().WantCaptureKeyboard) { notify_key_released(k); }
        break;
      case ButtonPress:
        imgui_api_backend::instance().on_button_pressed(b);
        if (!ImGui::GetIO().WantCaptureMouse) { notify_button_pressed(b); }
        break;
      case ButtonRelease:
        imgui_api_backend::instance().on_button_released(b);
        if (!ImGui::GetIO().WantCaptureMouse) { notify_button_released(b); }
        break;
      case MotionNotify:
        imgui_api_backend::instance().on_mouse_motion(m_xevent.xmotion.x,
                                                      m_xevent.xmotion.y);
        if (!ImGui::GetIO().WantCaptureMouse) {
          notify_mouse_motion(m_xevent.xmotion.x, m_xevent.xmotion.y);
        }
        break;
      case ConfigureNotify:
        imgui_api_backend::instance().on_resize(m_xevent.xconfigure.width,
                                                m_xevent.xconfigure.height);
        notify_resize(m_xevent.xconfigure.width, m_xevent.xconfigure.height);
        break;
    }
  }
}
//------------------------------------------------------------------------------
void window::init_glew() {
  glewExperimental = true;
  auto err         = glewInit();
  if (err != GLEW_OK) {
    throw std::runtime_error{std::string("[GLEW] cannot initialize: ") +
                             std::string((char *)glewGetErrorString(err))};
  }
}
//------------------------------------------------------------------------------
void window::setup(const std::string &title, GLsizei width, GLsizei height,
                   EGLint /*major*/, EGLint /*minor*/) {
  if (m_x_display == nullptr) {
    throw std::runtime_error{"[X] cannot connect server"};
  }
  if (m_egl_display == EGL_NO_DISPLAY) {
    throw std::runtime_error{"[EGL] could not get a display"};
  }
  EGLint egl_major, egl_minor;
  if (!eglInitialize(m_egl_display, &egl_major, &egl_minor)) {
    throw std::runtime_error{"[EGL] cannot initialize: " +
                             egl_error_to_string(eglGetError())};
  }

  // setup EGL
  EGLConfig config = nullptr;
  EGLint    num_configs_returned;
  EGLConfig egl_config;
  eglChooseConfig(m_egl_display, attribute_list.data(), &egl_config, 1,
                  &num_configs_returned);
  if (num_configs_returned != 1) {
    throw std::runtime_error{"[EGL] number of configs is not exactly one (" +
                             std::to_string(num_configs_returned) + ")."};
  }

  // get the visual from the EGL config
  XVisualInfo visual_template;
  visual_template.visualid = 0;
  int num_visuals_returned;
  if (!eglGetConfigAttrib(m_egl_display, config, EGL_NATIVE_VISUAL_ID,
                          (EGLint *)&visual_template.visualid)) {
    std::cerr << "[EGL] specified attribute list for config has no visual "
                 "id. Trying to found any other.\n";
    EGLint num_configs_returned = 0;
    if (!eglGetConfigs(m_egl_display, NULL, 0, &num_configs_returned)) {
      throw std::runtime_error{"[EGL] could not number of config lists."};
    }
    if (num_configs_returned == 0) {
      throw std::runtime_error{"[EGL] no configs in general have been found."};
    }
    std::cout << "number of configs found " << num_configs_returned << "\n";
    // now we create a buffer to store all our configs
    std::vector<EGLConfig> egl_configs(num_configs_returned);
    // and copy them into our buffer (don't forget to delete once done)
    if (!eglGetConfigs(m_egl_display, egl_configs.data(), num_configs_returned,
                       &num_configs_returned)) {
      throw std::runtime_error{"[EGL] could not get list of all configs."};
    }
    bool any = false;
    for (auto &egl_config : egl_configs) {
      EGLint buffer_size, red_size, blue_size, green_size, alpha_size,
          depth_size, surface_type;

      eglGetConfigAttrib(m_egl_display, egl_config, EGL_BUFFER_SIZE,
                         &buffer_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_RED_SIZE, &red_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_BLUE_SIZE, &green_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_GREEN_SIZE, &blue_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_ALPHA_SIZE,
                         &alpha_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_DEPTH_SIZE,
                         &depth_size);
      eglGetConfigAttrib(m_egl_display, egl_config, EGL_SURFACE_TYPE,
                         &surface_type);
      bool has_window_bit = (surface_type & EGL_WINDOW_BIT) > 0;
      if (buffer_size == 32 && has_window_bit && depth_size == 24 &&
          eglGetConfigAttrib(m_egl_display, egl_config, EGL_NATIVE_VISUAL_ID,
                             (EGLint *)&visual_template.visualid)) {
        config = egl_config;
        any    = true;
        break;
      }
    }
    if (!any) {
      throw std::runtime_error{
          "[EGL] could not find any framebuffer configuration for creating a "
          "window."};
    }
  }
  auto visual = XGetVisualInfo(m_x_display, VisualIDMask, &visual_template,
                               &num_visuals_returned);
  if (num_visuals_returned != 1) {
    throw std::runtime_error{"[X] number of visual infos is not exactly one (" +
                             std::to_string(num_visuals_returned) +
                             "). visual_id is " +
                             std::to_string(visual_template.visualid)};
  }

  Window root = DefaultRootWindow(m_x_display);
  // create a window
  XSetWindowAttributes window_attributes;
  window_attributes.event_mask =
      ExposureMask | KeyPressMask | ButtonPress | StructureNotifyMask |
      ButtonReleaseMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
      PointerMotionMask | Button1MotionMask | VisibilityChangeMask;
  window_attributes.colormap =
      XCreateColormap(m_x_display, root, visual->visual, AllocNone);

  m_x_window = XCreateWindow(m_x_display, root, 0, 0, width, height,
                             0,                         // border width
                             visual->depth,             // depth
                             InputOutput,               // class
                             visual->visual,            // visual
                             CWEventMask | CWColormap,  // attribute mask
                             &window_attributes         // attributes
  );
  XStoreName(m_x_display, m_x_window, title.c_str());

  // EGL context and surface
  eglBindAPI(EGL_OPENGL_API);
  //EGLint attrib_list[] = {EGL_CONTEXT_MAJOR_VERSION, major,
  //                        EGL_CONTEXT_MINOR_VERSION, minor, GL_NONE};
  m_egl_context =
      eglCreateContext(m_egl_display, config, EGL_NO_CONTEXT, nullptr);
  m_egl_surface =
      eglCreateWindowSurface(m_egl_display, config, m_x_window, nullptr);
  make_current();

  XFree(visual);

  XMapWindow(m_x_display, m_x_window);

  init_imgui(width, height);
}
//------------------------------------------------------------------------------
void window::swap_buffers() {
  eglSwapBuffers(m_egl_display, m_egl_surface);
}
//------------------------------------------------------------------------------
void window::init_imgui(size_t width, size_t height) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  m_imgui_render_backend = std::make_unique<imgui_render_backend>();
  imgui_api_backend::instance().on_resize(width, height);
}
//------------------------------------------------------------------------------
void window::deinit_imgui() {
  ImGui::DestroyContext();
}
//==============================================================================
}  // namespace yavin
//==============================================================================
