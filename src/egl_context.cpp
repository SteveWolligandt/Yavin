#include <yavin/egl_context.h>

//==============================================================================
namespace yavin {
//==============================================================================
context::context(EGLint major, EGLint minor)
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup(major, minor, EGL_NO_CONTEXT, true);
}
//------------------------------------------------------------------------------
context::context(EGLint major, EGLint minor, const context& parent)
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup(major, minor, parent.m_egl_context, false);
}
//------------------------------------------------------------------------------
context::~context() {
  eglTerminate(m_egl_display);
}
//==============================================================================
// methods
//==============================================================================
context context::create_shared_context(int major, int minor) const {
  return context{major, minor, *this};
}
//------------------------------------------------------------------------------
void context::make_current() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      m_egl_context)) {
    throw std::runtime_error{"[EGL] make context current."};
  }
  if (!m_glew_initialized) {
    init_glew();
    m_glew_initialized = true;
  }
}
//------------------------------------------------------------------------------
void context::release() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release context"};
  }
}
//------------------------------------------------------------------------------
void context::init_glew() {
  glewExperimental = true;
  auto err         = glewInit();
  if (GLEW_OK != err) {
    throw std::runtime_error{std::string("cannot initialize GLEW: ") +
                             std::string((char*)glewGetErrorString(err))};
  }
}
//------------------------------------------------------------------------------
void context::setup(EGLint major, EGLint minor, EGLContext parent, bool cur) {
  if (m_egl_display == EGL_NO_DISPLAY) {
    throw std::runtime_error{"[EGL] could not get a display"};
  }
  if (!eglInitialize(m_egl_display, &major, &minor)) {
    throw std::runtime_error{"[EGL] failed to initialize"};
  }

  EGLint    num_cfgs;
  EGLConfig egl_cfg;

  if (!eglChooseConfig(m_egl_display, attribute_list.data(), &egl_cfg, 1,
                       &num_cfgs)) {
    throw std::runtime_error{"[EGL] failed to choose config"};
  }
  eglBindAPI(EGL_OPENGL_API);
  m_egl_context =
      eglCreateContext(m_egl_display, egl_cfg, parent, nullptr);

  std::cerr << "Created GL " << major << "." << minor << " egl::context\n";

  if (cur) { make_current(); }
}

//==============================================================================
}  // namespace yavin
//==============================================================================
