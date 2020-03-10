#include <cstring>
#include <iostream>

static constexpr auto glx_context_major_version_arb = 0x2091;
static constexpr auto glx_context_minor_version_arb = 0x2092;

#include <yavin/glincludes.h>
#define YAVIN_X11_CONTEXT_DONT_DELETE
#include <yavin/glx_window.h>
#include <yavin/x11buttons.h>
#include <yavin/x11keys.h>
//==============================================================================
namespace yavin {
//==============================================================================
std::list<window *> window::contexts;
bool                window::m_error_occured      = false;
const int           window::m_visual_attribs[23] = {GLX_X_RENDERABLE,
                                          GL_TRUE,
                                          GLX_DRAWABLE_TYPE,
                                          GLX_WINDOW_BIT,
                                          GLX_RENDER_TYPE,
                                          GLX_RGBA_BIT,
                                          GLX_X_VISUAL_TYPE,
                                          GLX_TRUE_COLOR,
                                          GLX_RED_SIZE,
                                          8,
                                          GLX_GREEN_SIZE,
                                          8,
                                          GLX_BLUE_SIZE,
                                          8,
                                          GLX_ALPHA_SIZE,
                                          8,
                                          GLX_DEPTH_SIZE,
                                          24,
                                          GLX_STENCIL_SIZE,
                                          8,
                                          GLX_DOUBLEBUFFER,
                                          GL_TRUE,
                                          None};
//==============================================================================
window::window(const std::string &title, GLsizei width,
               GLsizei height, int major, int minor)
    : m_display{XOpenDisplay(nullptr)}, m_context{0} {
  contexts.push_back(this);
  if (!m_display) { throw std::runtime_error{"Failed to open X m_display"}; }
  m_screen    = DefaultScreenOfDisplay(m_display);
  m_screen_id = DefaultScreen(m_display);

  // FBConfigs were added in GLX version 1.3.
  int glx_major, glx_minor;
  if (!glXQueryVersion(m_display, &glx_major, &glx_minor) ||
      ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
    throw std::runtime_error{"Invalid GLX version"};
  }

  int          fbcount;
  GLXFBConfig *fbc = glXChooseFBConfig(m_display, DefaultScreen(m_display),
                                       m_visual_attribs, &fbcount);
  if (!fbc) {
    throw std::runtime_error{"Failed to retrieve a framebuffer config\n"};
  }

  // Pick the FB config/visual with the most samples per pixel
  int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

  for (int i = 0; i < fbcount; ++i) {
    XVisualInfo *vi = glXGetVisualFromFBConfig(m_display, fbc[i]);
    if (vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(m_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
      glXGetFBConfigAttrib(m_display, fbc[i], GLX_SAMPLES, &samples);

      if (best_fbc < 0 || (samp_buf && samples > best_num_samp)) {
        best_fbc = i, best_num_samp = samples;
      }
      if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp) {
        worst_fbc = i, worst_num_samp = samples;
      }
    }
    XFree(vi);
  }

  GLXFBConfig bestFbc = fbc[best_fbc];

  // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
  XFree(fbc);

  // Get a visual
  XVisualInfo *vi = glXGetVisualFromFBConfig(m_display, bestFbc);

  XSetWindowAttributes swa;
  swa.colormap = m_colormap = XCreateColormap(
      m_display, RootWindow(m_display, vi->screen), vi->visual, AllocNone);
  swa.background_pixmap = None;
  swa.border_pixel      = 0;
  swa.event_mask        = ExposureMask | KeyPressMask | ButtonPress |
                   StructureNotifyMask | ButtonReleaseMask | KeyReleaseMask |
                   EnterWindowMask | LeaveWindowMask | PointerMotionMask |
                   Button1MotionMask | VisibilityChangeMask;

  m_window = XCreateWindow(m_display, RootWindow(m_display, vi->screen), 0, 0,
                           width, height, 0, vi->depth, InputOutput, vi->visual,
                           CWBorderPixel | CWColormap | CWEventMask, &swa);
  if (!m_window) { throw std::runtime_error{"Failed to create window."}; }

  // Done with the visual info data
  XFree(vi);
  XStoreName(m_display, m_window, title.c_str());
  XMapWindow(m_display, m_window);
  XClearWindow(m_display, m_window);
  XMapRaised(m_display, m_window);

  // Get the default screen's GLX extension list
  const char *glxExts =
      glXQueryExtensionsString(m_display, DefaultScreen(m_display));

  // NOTE: It is not necessary to create or make current to a window before
  // calling glXGetProcAddressARB
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB =
      (glXCreateContextAttribsARBProc)glXGetProcAddressARB(
          (const GLubyte *)"glXCreateContextAttribsARB");

  // Install an X error handler so the application won't exit if GL 3.0
  // window allocation fails.
  // Note this error handler is global.  All display connections in all threads
  // of a process use the same error handler, so be sure to guard against other
  // threads issuing X commands while this code is running.
  int (*oldHandler)(Display *, XErrorEvent *) =
      XSetErrorHandler(&error_handler_static);

  // Check for the GLX_ARB_create_context extension string and the function.
  // If either is not present, use GLX 1.3 context creation method.
  if (!extension_supported(glxExts, "GLX_ARB_create_context") ||
      !glXCreateContextAttribsARB) {
    m_context =
        glXCreateNewContext(m_display, bestFbc, GLX_RGBA_TYPE, 0, GL_TRUE);
  } else {
    int context_attribs[] = {glx_context_major_version_arb, major,
                             glx_context_minor_version_arb, minor,
                             // GLX_CONTEXT_FLAGS_ARB        ,
                             // GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                             None};

    m_context = glXCreateContextAttribsARB(m_display, bestFbc, 0, GL_TRUE,
                                           context_attribs);

    // Sync to ensure any errors generated are processed.
    XSync(m_display, GL_FALSE);
    if (!m_error_occured && m_context) {
      std::cerr << "Created GL " << major << "." << minor << " GLX window\n";
      make_current();

      glewExperimental  = true;
      GLenum glew_error = glewInit();
      if (GLEW_OK != glew_error) {
        throw std::runtime_error{
            std::string("cannot initialize GLEW: ") +
            std::string((char *)glewGetErrorString(glew_error))};
      }
    } else {
      // Couldn't create GL 3.0+ context.  Fall back to old-style 2.x context.
      // When a context version below 3.0+ is requested, implementations will
      // return the newest context version compatible with OpenGL versions less
      // than version 3.0+.
      // glx_context_major_version_arb = 1
      context_attribs[1] = 1;
      // glx_context_minor_version_arb = 0
      context_attribs[3] = 0;

      m_error_occured = false;

      std::cerr << "Failed to create GL " << major << "." << minor
                << " context ... using old-style GLX context\n";
      m_context = glXCreateContextAttribsARB(m_display, bestFbc, 0, GL_TRUE,
                                             context_attribs);
    }
  }

  // Sync to ensure any errors generated are processed.
  XSync(m_display, GL_FALSE);

  // Restore the original error handler
  XSetErrorHandler(oldHandler);

  if (m_error_occured || !m_context) {
    throw std::runtime_error{"Failed to create an OpenGL context"};
  }
  init_imgui();
}
//------------------------------------------------------------------------------
window::~window() {
  deinit_imgui();
  glXMakeCurrent(m_display, 0, 0);
  glXDestroyContext(m_display, m_context);

  XDestroyWindow(m_display, m_window);
  XFreeColormap(m_display, m_colormap);
  XCloseDisplay(m_display);
  contexts.remove(this);
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
  while (XCheckWindowEvent(m_display, m_window,
                           ExposureMask | KeyPressMask | ButtonPress |
                               StructureNotifyMask | ButtonReleaseMask |
                               KeyReleaseMask | EnterWindowMask |
                               LeaveWindowMask | PointerMotionMask |
                               Button1MotionMask | VisibilityChangeMask,
                           &m_xevent)) {
    const auto k = x11_keysym_to_key(
        XkbKeycodeToKeysym(m_display, m_xevent.xkey.keycode, 0, 0));
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
        if (!ImGui::GetIO().WantCaptureMouse) {notify_button_pressed(b);}
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
bool window::extension_supported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;

  // Extension names should not have spaces.
  where = strchr(extension, ' ');
  if (where || *extension == '\0') return false;

  // It takes a bit of care to be fool-proof about parsing the OpenGL extensions
  // string. Don't be fooled by sub-strings, etc.
  for (start = extList;;) {
    where = strstr(start, extension);
    if (!where) break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0') return true;
    start = terminator;
  }
  return false;
}
//------------------------------------------------------------------------------
int window::error_handler_static(Display *m_display, XErrorEvent *ev) {
  for (auto m_context : contexts)
    if (m_context->m_display == m_display) return m_context->error_handler(ev);
  return 0;
}
//------------------------------------------------------------------------------
int window::error_handler(XErrorEvent * /*ev*/) {
  m_error_occured = true;
  return 0;
}
//------------------------------------------------------------------------------
void window::swap_buffers() {
  glXSwapBuffers(m_display, m_window);
}
//------------------------------------------------------------------------------
void window::init_imgui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  m_imgui_render_backend = std::make_unique<imgui_render_backend>();
}
//------------------------------------------------------------------------------
void window::deinit_imgui() {
  ImGui::DestroyContext();
}
//==============================================================================
}  // namespace yavin
//==============================================================================

