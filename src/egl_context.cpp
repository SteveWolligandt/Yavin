#include <yavin/egl_context.h>
#include <yavin/egl_window.h>
//==============================================================================
namespace yavin {
//==============================================================================
context::context() : m_egl_env{std::make_shared<egl::environment>()} {
  setup(EGL_NO_CONTEXT, true);
}
//------------------------------------------------------------------------------
context::context(window const& parent)
    : m_egl_env{parent.m_egl_env} {
  setup(parent.m_egl_context->get(), false);
}
//==============================================================================
// methods
//==============================================================================
void context::make_current() {
  if (!eglMakeCurrent(m_egl_env->display()->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, m_egl_context->get())) {
    throw std::runtime_error{"[EGL] make context current."};
  }
  if (m_glew == nullptr) { m_glew = glew::create(); }
}
//------------------------------------------------------------------------------
void context::release() {
  if (!eglMakeCurrent(m_egl_env->display()->get(), EGL_NO_SURFACE,
                      EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
    throw std::runtime_error{"[EGL] could not release context"};
  }
}
//------------------------------------------------------------------------------
void context::setup(EGLContext parent, bool cur) {
  eglBindAPI(EGL_OPENGL_API);
  EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  m_egl_context = std::make_shared<egl::context>(m_egl_env->display(), parent,
                                                 context_attributes);
  if (cur) { make_current(); }
}
//==============================================================================
}  // namespace yavin
//==============================================================================
