#include <yavin/egl_window.h>

//==============================================================================
namespace yavin {
//==============================================================================
window::window(const std::string &title, GLsizei width, GLsizei height,
               EGLint major, EGLint minor)
    : x_display{XOpenDisplay(nullptr)},
      m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup(major, minor, true);
}
//------------------------------------------------------------------------------
window::~window() {
  eglTerminate(m_egl_display);
  XDestroyWindow(x_display, win);
  XCloseDisplay(x_display);
}
//==============================================================================
// methods
//==============================================================================
//context window::create_shared_context(EGLint major, EGLint minor) const {
//  return context{major, minor, *this};
//}
//------------------------------------------------------------------------------
void window::make_current() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      m_egl_window)) {
    throw std::runtime_error{"[EGL] make window current."};
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
void window::init_glew() {
  glewExperimental = true;
  auto err         = glewInit();
  if (GLEW_OK != err) {
    throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                             std::string((char *)glewGetErrorString(err))};
  }
}
//------------------------------------------------------------------------------
void window::setup(const std::string &title, GLsizei width, GLsizei height,
                   EGLint major, EGLint minor, bool cur) {
  if (m_egl_display == EGL_NO_DISPLAY) {
    throw std::runtime_error{"[EGL] could not get a display"};
  }
  if (!eglInitialize(m_egl_display, &major, &minor)) {
    throw std::runtime_error{"[EGL] failed to initialize"};
  }
  if (x_display == nullptr) {
    throw std::runtime_error{"cannot connect to X server"};
  }
  setup_x();
  setup_egl();
}
//------------------------------------------------------------------------------
void window::setup_x(const std::string &title, GLsizei width, GLsizei height) {
  Window root = DefaultRootWindow(x_display);

  XSetWindowAttributes swa;
  swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;

  win = XCreateWindow(x_display, root, 0, 0, width, height, 0, CopyFromParent,
                      InputOutput, CopyFromParent, CWEventMask, &swa);

  XSetWindowAttributes xattr;
  Atom                 atom;
  int                  one = 1;

  xattr.override_redirect = False;
  XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

  atom = XInternAtom(x_display, "_NET_WM_STATE_FULLSCREEN", True);
  XChangeProperty(x_display, win, XInternAtom(x_display, "_NET_WM_STATE", True),
                  XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom, 1);

  XChangeProperty(
      x_display, win,
      XInternAtom(x_display, "_HILDON_NON_COMPOSITED_WINDOW", False),
      XA_INTEGER, 32, PropModeReplace, (unsigned char *)&one, 1);

  XWMHints hints;
  hints.input = True;
  hints.flags = InputHint;
  XSetWMHints(x_display, win, &hints);

  XMapWindow(x_display, win);  // make the window visible on the screen
  XStoreName(x_display, win, "GL test");  // give the window a name

  // get identifiers for the provided atom name strings
  Atom wm_state   = XInternAtom(x_display, "_NET_WM_STATE", False);
  Atom fullscreen = XInternAtom(x_display, "_NET_WM_STATE_FULLSCREEN", False);

  XEvent xev;
  memset(&xev, 0, sizeof(xev));

  xev.type                 = ClientMessage;
  xev.xclient.window       = win;
  xev.xclient.message_type = wm_state;
  xev.xclient.format       = 32;
  xev.xclient.data.l[0]    = 1;
  xev.xclient.data.l[1]    = fullscreen;
  XSendEvent(  // send an event mask to the X-server
      x_display, DefaultRootWindow(x_display), False, SubstructureNotifyMask,
      &xev);
}
//------------------------------------------------------------------------------
void window::setup_egl(EGLint major, EGLint minor, bool cur) {
  EGLint    num_cfgs;
  EGLConfig egl_cfg;

  if (!eglChooseConfig(m_egl_display, attribute_list.data(), &egl_cfg, 1,
                       &num_cfgs)) {
    throw std::runtime_error{"[EGL] failed to choose config"};
  }
  eglBindAPI(EGL_OPENGL_API);
  m_egl_window =
      eglCreateContext(m_egl_display, egl_cfg, EGL_NO_CONTEXT, nullptr);

  std::cerr << "Created GL " << major << "." << minor << " egl::window\n";

  if (cur) { make_current(); }
}

//==============================================================================
}  // namespace yavin
//==============================================================================
