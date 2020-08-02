#include <yavin/egl_context.h>
#include <yavin/egl_window.h>
//==============================================================================
namespace yavin {
//==============================================================================
context::context()
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup( EGL_NO_CONTEXT, true);
}
//------------------------------------------------------------------------------
context::context( context const & parent)
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup( parent.m_egl_context, false);
}
//------------------------------------------------------------------------------
context::context( window const& parent)
    : m_egl_display{eglGetDisplay(EGL_DEFAULT_DISPLAY)} {
  setup( parent.m_egl_context->get(), false);
}
//------------------------------------------------------------------------------
context::~context() {
  eglTerminate(m_egl_display);
}
//==============================================================================
// methods
//==============================================================================
context context::create_shared_context() const {
  return context{ *this};
}
//------------------------------------------------------------------------------
void context::make_current() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      m_egl_context)) {
    throw std::runtime_error{"[EGL] make context current."};
  }
  if (m_glew == nullptr) { m_glew = glew::create(); }
}
//------------------------------------------------------------------------------
void context::release() {
  if (!eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                      EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release context"};
  }
}
//------------------------------------------------------------------------------
void context::setup( EGLContext parent, bool cur) {
  if (m_egl_display == EGL_NO_DISPLAY) {
    throw std::runtime_error{"[EGL] could not get a display"};
  }
  EGLint major, minor;
  if (!eglInitialize(m_egl_display, &major, &minor)) {
    throw std::runtime_error{"[EGL] failed to initialize"};
  }

  EGLint    num_cfgs;
  EGLConfig egl_config;

  if (!eglChooseConfig(m_egl_display, attribute_list.data(), &egl_config, 1,
                       &num_cfgs)) {
    throw std::runtime_error{"[EGL] failed to choose config"};
  }
  eglBindAPI(EGL_OPENGL_API);
  EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  m_egl_context =
      eglCreateContext(m_egl_display, egl_config, parent, context_attributes);
  if (cur) { make_current(); }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
